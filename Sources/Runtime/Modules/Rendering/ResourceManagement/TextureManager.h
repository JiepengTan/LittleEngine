/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Resources/Loaders/TextureLoader.h"

#include "Modules/Rendering/ResourceManagement/AResourceManager.h"

namespace LittleEngine::ResourceManagement
{
	/**
	* ResourceManager of textures
	*/
	class TextureManager : public AResourceManager<LittleEngine::Rendering::Resources::Texture>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual LittleEngine::Rendering::Resources::Texture* CreateResource(const std::string & p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(LittleEngine::Rendering::Resources::Texture* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(LittleEngine::Rendering::Resources::Texture* p_resource, const std::string& p_path) override;
	};
}
