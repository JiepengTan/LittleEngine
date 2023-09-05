/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/ResourceManagement/SoundManager.h"

LittleEngine::Audio::Resources::Sound* LittleEngine::ResourceManagement::SoundManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);
	LittleEngine::Audio::Resources::Sound* sound = LittleEngine::Audio::Resources::Loaders::SoundLoader::Create(realPath);
	if (sound)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(sound) + offsetof(LittleEngine::Audio::Resources::Sound, path)) = p_path; // Force the resource path to fit the given path

	return sound;
}

void LittleEngine::ResourceManagement::SoundManager::DestroyResource(LittleEngine::Audio::Resources::Sound* p_resource)
{
	LittleEngine::Audio::Resources::Loaders::SoundLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::SoundManager::ReloadResource(LittleEngine::Audio::Resources::Sound* p_resource, const std::string& p_path)
{
}
