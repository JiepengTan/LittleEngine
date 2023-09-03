/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include "Resource/Rendering/Resources/Parsers/AssimpParser.h"

#include <array>
#include <cassert>
#include <assimp/postprocess.h>
#include "Core/Debug/Assertion.h"
#include "Core/Debug/Logger.h"
#include "Resource/Rendering/Resources/AnimationData.h"
#include "Resource/Rendering/Resources/Animation.h"
#include "Resource/Rendering/Resources/Model.h"

static OvMaths::FMatrix4 ConvertMatrixToGLMFormat(const aiMatrix4x4& p_from)
{
    return *((OvMaths::FMatrix4*)(&p_from));
}

void OvRendering::Resources::Parsers::AssimpParser::ReadMissingBones(OvRendering::Resources::Animation* p_anim,
                                                                     aiAnimation* animation)
{
    auto& boneInfoMap = *p_anim->GetBoneInfoMap(); //getting m_BoneInfoMap from Model class
    int& boneCount = *p_anim->GetBoneCounter(); //getting the m_BoneCounter from Model class
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
            if (boneName != "root")
            {
                OVLOG("Miss Animation Node " + boneName);
            }
        }
        p_anim->AddBone(BoneFrames(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }
}

bool OvRendering::Resources::Parsers::AssimpParser::LoadAnimation(Animation* p_anim, const std::string& p_fileName,
                                                                  EModelParserFlags p_parserFlags)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(p_fileName, aiProcess_ValidateDataStructure | aiProcess_GlobalScale);
    if (scene == nullptr || scene->mAnimations == nullptr)
    {
        OVLOG_ERROR("Can not load animtion file " + p_fileName);
        return false;
    }
    auto animation = scene->mAnimations[0];
    p_anim->SetDuration((float)animation->mDuration);
    p_anim->SetTicksPerSecond((int)animation->mTicksPerSecond);
    aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
    globalTransformation = globalTransformation.Inverse();
    m_name2Bone.clear();
    ReadHierarchyData(p_anim->GetSkeletonRoot(), scene->mRootNode);
    // "$AssimpFbx$_Pre" is a special name , why need it?
    ReadMissingBones(p_anim, animation);
    return true;
}


void OvRendering::Resources::Parsers::AssimpParser::ReadHierarchyData(SkeletonBone& p_dest, const aiNode* p_src)
{
    p_dest.name = p_src->mName.data;
    p_dest.transformation = ConvertMatrixToGLMFormat(p_src->mTransformation); // 直接内存映射
    p_dest.childrenCount = p_src->mNumChildren;

    for (uint32_t i = 0; i < p_src->mNumChildren; i++)
    {
        SkeletonBone newData;
        ReadHierarchyData(newData, p_src->mChildren[i]);
        p_dest.children.push_back(newData);
    }
    m_name2Bone[p_dest.name] = p_dest;
}


bool OvRendering::Resources::Parsers::AssimpParser::LoadModel(Model* p_model, const std::string& p_fileName,
                                                              std::vector<Mesh*>& p_meshes,
                                                              std::vector<std::string>& p_materials,
                                                              EModelParserFlags p_parserFlags)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(
        p_fileName, aiProcess_Triangulate | aiProcess_GlobalScale | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        return false;

    ProcessMaterials(scene, p_materials);

    aiMatrix4x4 identity;

    ProcessNode(p_model, &identity, scene->mRootNode, scene, p_meshes);

    bool hasBone = false;
    for (auto mesh : p_meshes)
    {
        hasBone |= mesh->isSkinMesh;
    }
    p_model->isSkinMesh = hasBone;
    return true;
}

void OvRendering::Resources::Parsers::AssimpParser::ProcessMaterials(const aiScene* p_scene,
                                                                     std::vector<std::string>& p_materials)
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

void OvRendering::Resources::Parsers::AssimpParser::ProcessNode(Model* p_model, void* p_transform, aiNode* p_node,
                                                                const aiScene* p_scene, std::vector<Mesh*>& p_meshes)
{
    aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform) * p_node->mTransformation;

    // Process all the node's meshes (if any)
    for (uint32_t i = 0; i < p_node->mNumMeshes; ++i)
    {
        std::vector<uint32_t> indices;
        aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
        auto flag = mesh->HasBones() ? Geometry::vdf_all : Geometry::vdf_allNoBone;
        Geometry::VertexDataBuffer vertices(mesh->mNumVertices, flag);
        ProcessMesh(p_model, &nodeTransformation, mesh, p_scene, vertices, indices);
        auto meshPtr = new Mesh(vertices, indices, mesh->mMaterialIndex, flag, mesh->HasBones());
        p_meshes.push_back(meshPtr); // The model will handle mesh destruction
    }

    // Then do the same for each of its children
    for (uint32_t i = 0; i < p_node->mNumChildren; ++i)
    {
        ProcessNode(p_model, &nodeTransformation, p_node->mChildren[i], p_scene, p_meshes);
    }
}




void OvRendering::Resources::Parsers::AssimpParser::ProcessVertexBoneInfo(OvRendering::Resources::Model* p_model, aiMesh* p_mesh, OvRendering::Geometry::VertexDataBuffer& p_verticesBuffer)
{
    if(p_mesh->mNumBones ==0) return;
    //1. calc vertex's boneId and weights
    auto& boneInfoMap = p_model->GetBoneInfoMap();
    int& boneCount = p_model->GetBoneCount();

    auto weightPtr = (float*)p_verticesBuffer.GetDataPtr(Geometry::EVertexDataFlagsIndex::boneWeight);
    auto boneIdPtr = (int*)p_verticesBuffer.GetDataPtr(Geometry::EVertexDataFlagsIndex::boneId);
    auto boneIdAryLen = p_verticesBuffer.dataLen[(int)Geometry::EVertexDataFlagsIndex::boneWeight];
    for (uint32_t boneIndex = 0; boneIndex < p_mesh->mNumBones; ++boneIndex)
    {
        int boneID;
        std::string boneName = p_mesh->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName] = {
                boneCount,
                ConvertMatrixToGLMFormat(p_mesh->mBones[boneIndex]->mOffsetMatrix)
            };
            boneID = boneCount;
            boneCount++;
        }
        else
        {
            boneID = boneInfoMap[boneName].id;
        }

        auto weights = p_mesh->mBones[boneIndex]->mWeights;
        int numWeights = p_mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            //SetVertexBoneData(vertices[vertexId], boneID, weight);
            if (vertexId >= p_verticesBuffer.Size())
            {
                OVLOG_ERROR("Model file animation invalid !" + p_model->path);
                return;
            }
            auto startOffset = vertexId * MAX_BONE_INFLUENCE;
            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
            {
                auto curOffset = startOffset + i;
                OVASSERT(boneIdAryLen > curOffset, "ProcessVertexBoneInfo outOfRange ");
                if (boneIdPtr[curOffset] < 0)
                {
                    boneIdPtr[curOffset] = boneID;
                    weightPtr[curOffset] = weight;
                    break;
                }
            }
        }
    }
    //2. normalize bone weights
    auto vertexCount = p_mesh->mNumVertices;
    for (uint32_t idx = 0; idx < vertexCount; idx++)
    {
        auto startOffset = idx * MAX_BONE_INFLUENCE;
        float sumWeight = 0;
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            sumWeight += weightPtr[startOffset + i];
        }
        if (sumWeight > 0.001f)
        {
            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
            {
                weightPtr[startOffset + i] /= sumWeight;
            }
        }
    }
}

void OvRendering::Resources::Parsers::AssimpParser::ProcessMesh(OvRendering::Resources::Model* p_model,
                                                                void* p_transform,
                                                                aiMesh* p_mesh, const aiScene* p_scene,
                                                                Geometry::VertexDataBuffer& p_verticesBuffer,
                                                                std::vector<uint32_t>& p_outIndices)
{
    aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(p_transform);
    //1. read basic mesh data
    for (uint32_t i = 0; i < p_mesh->mNumVertices; ++i)
    {
        aiVector3D position		= meshTransformation * p_mesh->mVertices[i];
        aiVector3D normal		= meshTransformation * (p_mesh->mNormals ? p_mesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
        aiVector3D texCoords	= p_mesh->mTextureCoords[0] ? p_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
        aiVector3D tangent		= p_mesh->mTangents ? meshTransformation * p_mesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
        aiVector3D bitangent	= p_mesh->mBitangents ? meshTransformation * p_mesh->mBitangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
        aiColor4D  color	    = p_mesh->mColors[0] != nullptr ?  p_mesh->mColors[0][i] : aiColor4D(0.0f, 0.0f, 0.0f,0.0f);

        p_verticesBuffer.SetPosition(i, position.x, position.y, position.z);
        p_verticesBuffer.SetTexCoord(i, texCoords.x, texCoords.y);
        p_verticesBuffer.SetColor(i, color.r, color.g, color.b, color.a);
        p_verticesBuffer.SetNormal(i, normal.x, normal.y, normal.z);
        p_verticesBuffer.SetTangent(i, tangent.x, tangent.y, tangent.z);
        p_verticesBuffer.SetBitangent(i, bitangent.x, bitangent.y, bitangent.z);
        if (p_mesh->mNumBones > 0)
        {
            p_verticesBuffer.SetBoneId(i, -1, -1, -1, -1);
            p_verticesBuffer.SetBoneWeight(i, 0, 0, 0, 0);
        }
    }

    for (uint32_t faceID = 0; faceID < p_mesh->mNumFaces; ++faceID)
    {
        auto& face = p_mesh->mFaces[faceID];
        for (size_t indexID = 0; indexID < 3; ++indexID)
            p_outIndices.push_back(face.mIndices[indexID]);
    }

    //2. extra bone info
    ProcessVertexBoneInfo(p_model, p_mesh, p_verticesBuffer);

}
