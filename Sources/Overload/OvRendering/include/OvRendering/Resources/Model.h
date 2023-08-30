/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>
#include <vector>
#include <string>
#include "OvRendering/Resources/AnimationData.h"
#include "OvRendering/Resources/Mesh.h"
namespace OvRendering::Resources::Loaders
{
	class ModelLoader;
}

namespace OvRendering::Resources
{

	/**
	* A model is a combinaison of meshes
	*/
	class Model
	{
		friend class OvRendering::Resources::Loaders::ModelLoader;

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
		const OvRendering::Geometry::BoundingSphere& GetBoundingSphere() const;

	private:
		Model(const std::string& p_path);
		~Model();

		void ComputeBoundingSphere();

	public:
		const std::string path;
		int& GetBoneCount() { return m_BoneCounter; }
		std::map<std::string, OvRendering::Resources::BoneInfo>& GetBoneInfoMap() { return m_name2BoneInfo; }
		
	private:
		std::vector<Mesh*> m_meshes;
		std::vector<std::string> m_materialNames;

		Geometry::BoundingSphere m_boundingSphere;
		std::map<std::string, OvRendering::Resources::BoneInfo> m_name2BoneInfo;
		int m_BoneCounter = 0;
	};
}