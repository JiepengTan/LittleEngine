/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/
#pragma once

#include <map>
#include <vector>

#include "Core/Maths/FMatrix4.h"
#include "Resource/Rendering/Geometry/Vertex.h"
#include "Resource/Rendering/Resources/Parsers/IModelParser.h"

namespace OvRendering::Resources
{
	class Animation;
	class Model;
	class Mesh;
	struct SkeletonBone;
}
struct aiAnimation;
struct aiMesh;
struct aiNode;
namespace OvRendering::Resources::Parsers
{
	/**
	* A simple class to load assimp model data (Vertices only)
	*/
	class AssimpParser : public IModelParser
	{
	public:
		/**
		* Simply load meshes from a file using assimp
		* Return true on success
		* @param p_filename
		* @param p_meshes
		* @param p_parserFlags
		*/
		bool LoadModel
		(
			Model* p_model,
			const std::string& p_fileName,
			std::vector<Mesh*>& p_meshes,
			std::vector<std::string>& p_materials,
			EModelParserFlags p_parserFlags
		) override;
		bool LoadAnimation(Animation* p_anim, const std::string& p_fileName,EModelParserFlags p_parserFlags);
	private:
		void ReadMissingBones(OvRendering::Resources::Animation* p_anim, aiAnimation* animation);
		void ReadHierarchyData(SkeletonBone& p_dest, const struct aiNode* src);
		void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);;
		void ProcessNode(Model* p_model,void* p_transform, struct aiNode* p_node, const struct aiScene* p_scene, std::vector<Mesh*>& p_meshes);
		void NormalizedBoneWeights(float* weightPtr, unsigned vertexCount);
		void ProcessVertexBoneInfo(OvRendering::Resources::Model* p_model, aiMesh* p_mesh,OvRendering::Geometry::VertexDataBuffer& p_verticesBuffer);
		void NormalizedBoneWeights(aiMesh* p_mesh, float* weightPtr);
		void ProcessMesh(OvRendering::Resources::Model* p_model,void* p_transform, struct aiMesh* p_mesh, const struct aiScene* p_scene,
		                 Geometry::VertexDataBuffer& p_verticesBuffer, std::vector<uint32_t>& p_outIndices);
	
	private:
		std::map<std::string, SkeletonBone> m_name2Bone ;
		const int MAX_BONE_INFLUENCE = 4;
	};
}
