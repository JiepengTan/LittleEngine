/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/
#pragma once

#include <map>
#include <vector>

#include "OvMaths/FMatrix4.h"
#include "OvRendering/Geometry/Vertex.h"
#include "OvRendering/Resources/Parsers/IModelParser.h"

namespace OvRendering::Resources
{
	class Animation;
	class Model;
	class Mesh;
	struct SkeletonBone;
}
class aiAnimation;
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
		void ReadMissingBones(OvRendering::Resources::Animation* p_anim, aiAnimation* animation);
		bool LoadAnimation(Animation* p_anim, const std::string& p_fileName,EModelParserFlags p_parserFlags);
	private:
		void ReadHierarchyData(SkeletonBone& p_dest, const struct aiNode* src);
		void ProcessMaterials(const struct aiScene* p_scene, std::vector<std::string>& p_materials);;
		void ProcessNode(Model* p_model,void* p_transform, struct aiNode* p_node, const struct aiScene* p_scene, std::vector<Mesh*>& p_meshes);
		void ProcessMesh(OvRendering::Resources::Model* p_model,void* p_transform, struct aiMesh* p_mesh, const struct aiScene* p_scene, std::vector<Geometry::Vertex>& p_outVertices, std::vector<uint32_t>& p_outIndices);
		void SetVertexBoneData(Geometry::Vertex& vertex, int boneID, float weight);
		void ExtractBoneWeightForVertices(OvRendering::Resources::Model* p_model, std::vector<Geometry::Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	
	private:
		std::map<std::string, SkeletonBone> name2Bone ;
		const int MAX_BONE_INFLUENCE = 4;
	};
}