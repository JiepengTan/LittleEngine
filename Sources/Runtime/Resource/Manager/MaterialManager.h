/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Resources/Loaders/MaterialLoader.h"
#include "Modules/Rendering/ResourceManagement/AResourceManager.h"

namespace LittleEngine::ResourceManagement
{
	/**
	* ResourceManager of materials
	*/
	class MaterialManager : public AResourceManager<LittleEngine::Resources::Material>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual LittleEngine::Resources::Material* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(LittleEngine::Resources::Material* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(LittleEngine::Resources::Material* p_resource, const std::string& p_path) override;
	};
}
