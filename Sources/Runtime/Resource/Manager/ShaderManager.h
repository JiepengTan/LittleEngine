/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/Loader/ShaderLoader.h"

#include "Resource/Manager/AResourceManager.h"

namespace LittleEngine::ResourceManagement
{
	/**
	* ResourceManager of shaders
	*/
	class ShaderManager : public AResourceManager<LittleEngine::Rendering::Resources::Shader>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual LittleEngine::Rendering::Resources::Shader* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(LittleEngine::Rendering::Resources::Shader* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(LittleEngine::Rendering::Resources::Shader* p_resource, const std::string& p_path) override;
	};
}
