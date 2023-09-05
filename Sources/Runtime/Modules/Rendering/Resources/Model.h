/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>
#include <vector>
#include <string>
#include "Modules/Rendering/Resources/AnimationData.h"
#include "Modules/Rendering/Resources/Mesh.h"
namespace LittleEngine::Rendering::Resources::Loaders
{
	class ModelLoader;
}

namespace LittleEngine::Rendering::Resources
{

	/**
	* A model is a combinaison of meshes
	*/
	class Model
	{
		friend class LittleEngine::Rendering::Resources::Loaders::ModelLoader;

	public:
		/**
		* Returns the meshes
		*/
		const std::vector<Mesh*>& GetMeshes() const;

		/**
		* Returns the material names
		*/
		const std::vector<std::string>& GetMaterialNames() const;

		/**
		* Returns the bounding sphere of the model
		*/
		const LittleEngine::Rendering::Geometry::BoundingSphere& GetBoundingSphere() const;

	private:
		Model(const std::string& p_path);
		~Model();

		void ComputeBoundingSphere();

	public:
		const std::string path;
		int& GetBoneCount() { return m_BoneCounter; }
		std::map<std::string, LittleEngine::Rendering::Resources::BoneInfo>& GetBoneInfoMap() { return m_name2BoneInfo; }
		bool isSkinMesh;
		
	private:
		std::vector<Mesh*> m_meshes;
		std::vector<std::string> m_materialNames;

		Geometry::BoundingSphere m_boundingSphere;
		std::map<std::string, LittleEngine::Rendering::Resources::BoneInfo> m_name2BoneInfo;
		int m_BoneCounter = 0;
	};
}
