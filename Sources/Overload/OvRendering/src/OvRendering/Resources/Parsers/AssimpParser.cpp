/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include "OvRendering/Resources/Parsers/AssimpParser.h"

#include <cassert>
#include <assimp/postprocess.h>
#include "OvDebug/Assertion.h"
#include "OvDebug/Logger.h"
#include "OvRendering/Resources/AnimationData.h"
#include "OvRendering/Resources/Animation.h"
#include "OvRendering/Resources/Model.h"

static OvMaths::FMatrix4 ConvertMatrixToGLMFormat(const aiMatrix4x4& p_from)
{
	return *((OvMaths::FMatrix4*)(&p_from));
}

bool OvRendering::Resources::Parsers::AssimpParser::LoadAnimation(Animation* p_anim, const std::string& p_fileName,EModelParserFlags p_parserFlags)
{
	Assimp::Importer importer;
	auto flags  = aiProcess_Triangulate | aiProcess_GenNormals |  aiProcess_JoinIdenticalVertices;
	const aiScene* scene = importer.ReadFile(p_fileName, aiProcess_Triangulate);
	if(scene == nullptr || scene->mAnimations == nullptr)
	{
		OVLOG_ERROR("Can not load animtion file " + p_fileName);
		return false;
	}
	auto animation = scene->mAnimations[0];
	p_anim->m_Duration = animation->mDuration;
	p_anim->m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHierarchyData(p_anim->m_skeletonRoot, scene->mRootNode);
	
	auto& boneInfoMap = p_anim->GetBoneInfoMap(); //getting m_BoneInfoMap from Model class
	int& boneCount = p_anim->GetBoneCount(); //getting the m_BoneCounter from Model class
	// read all channel 
	int size = animation->mNumChannels;
	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		p_anim->m_Bones.push_back(BoneFrames(channel->mNodeName.data,
							   boneInfoMap[channel->mNodeName.data].id, channel));
	}
	p_anim->m_name2BoneInfo = boneInfoMap;
	return true;
}
void OvRendering::Resources::Parsers::AssimpParser::ReadHierarchyData(SkeletonBone& p_dest, const aiNode* p_src)
{
	p_dest.name = p_src->mName.data;
	p_dest.transformation = ConvertMatrixToGLMFormat(p_src->mTransformation); // 直接内存映射
	p_dest.childrenCount = p_src->mNumChildren;

	for (int i = 0; i < p_src->mNumChildren; i++)
	{
		SkeletonBone newData;
		ReadHierarchyData(newData, p_src->mChildren[i]);
		p_dest.children.push_back(newData);
	}
}


bool OvRendering::Resources::Parsers::AssimpParser::LoadModel(Model* p_model, const std::string & p_fileName, std::vector<Mesh*>& p_meshes, std::vector<std::string>& p_materials, EModelParserFlags p_parserFlags)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(p_fileName, static_cast<unsigned int>(p_parserFlags));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return false;

	ProcessMaterials(scene, p_materials);

	aiMatrix4x4 identity;

	ProcessNode(p_model,&identity, scene->mRootNode, scene, p_meshes);

	return true;
}

void OvRendering::Resources::Parsers::AssimpParser::ProcessMaterials(const aiScene * p_scene, std::vector<std::string>& p_materials)
{
	for (uint32_t i = 0; i < p_scene->mNumMaterials; ++i)
	{
		aiMaterial* material = p_scene->mMaterials[i];
		if (material)
		{
			aiString name;
			aiGetMaterialString(material, AI_MATKEY_NAME, &name);
			p_materials.push_back(name.C_Str());
		}
	}
}

void OvRendering::Resources::Parsers::AssimpParser::ProcessNode(Model* p_model,void* p_transform, aiNode * p_node, const aiScene * p_scene, std::vector<Mesh*>& p_meshes)
{
	aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform) * p_node->mTransformation;

	// Process all the node's meshes (if any)
	for (uint32_t i = 0; i < p_node->mNumMeshes; ++i)
	{
		std::vector<Geometry::Vertex> vertices;
		std::vector<uint32_t> indices;
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		ProcessMesh(p_model, &nodeTransformation, mesh, p_scene, vertices, indices);
		p_meshes.push_back(new Mesh(vertices, indices, mesh->mMaterialIndex)); // The model will handle mesh destruction
	}

	// Then do the same for each of its children
	for (uint32_t i = 0; i < p_node->mNumChildren; ++i)
	{
		ProcessNode(p_model, &nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
	}
}
void OvRendering::Resources::Parsers::AssimpParser::SetVertexBoneData(Geometry::Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.boneIds[i] < 0)
		{
			vertex.boneWeights[i] = weight;
			vertex.boneIds[i] = boneID;
			break;
		}
	}
}

void OvRendering::Resources::Parsers::AssimpParser::ExtractBoneWeightForVertices(OvRendering::Resources::Model* p_model,std::vector<Geometry::Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	auto& boneInfoMap = p_model->GetBoneInfoMap();
	int& boneCount = p_model->GetBoneCount();

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName] ={
				boneCount,
				ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix)
			};
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			OVASSERT(vertexId <= vertices.size()," Model file invalid !");
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void OvRendering::Resources::Parsers::AssimpParser::ProcessMesh(OvRendering::Resources::Model* p_model,void* p_transform, aiMesh* p_mesh, const aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices)
{
	aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform);

	for (uint32_t i = 0; i < p_mesh->mNumVertices; ++i)
	{
		aiVector3D position		= meshTransformation * p_mesh->mVertices[i];
		aiVector3D normal		= meshTransformation * (p_mesh->mNormals ? p_mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
		aiVector3D texCoords	= p_mesh->mTextureCoords[0] ? p_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D tangent		= p_mesh->mTangents ? meshTransformation * p_mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
		aiVector3D bitangent	= p_mesh->mBitangents ? meshTransformation * p_mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);

		p_outVertices.push_back
		(
			{
				position.x,
				position.y,
				position.z,
				texCoords.x,
				texCoords.y,
				normal.x,
				normal.y,
				normal.z,
				tangent.x,
				tangent.y,
				tangent.z,
				bitangent.x,
				bitangent.y,
				bitangent.z
			}
		);
	}

	for (uint32_t faceID = 0; faceID < p_mesh->mNumFaces; ++faceID)
	{
		auto& face = p_mesh->mFaces[faceID];

		for (size_t indexID = 0; indexID < 3; ++indexID)
			p_outIndices.push_back(face.mIndices[indexID]);
	}
	ExtractBoneWeightForVertices(p_model,p_outVertices,p_mesh,p_scene);
}
