/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/Loader//SoundLoader.h"

#include "Resource/Manager/AResourceManager.h"

namespace LittleEngine::ResourceManagement
{
	/**
	* ResourceManager of sounds
	*/
	class SoundManager : public AResourceManager<LittleEngine::Sound>
	{
	public:
		/**
		* Create the resource identified by the given path
		* @param p_path
		*/
		virtual LittleEngine::Sound* CreateResource(const std::string& p_path) override;

		/**
		* Destroy the given resource
		* @param p_resource
		*/
		virtual void DestroyResource(LittleEngine::Sound* p_resource) override;

		/**
		* Reload the given resource
		* @param p_resource
		* @param p_path
		*/
		virtual void ReloadResource(LittleEngine::Sound* p_resource, const std::string& p_path) override;
	};
}
