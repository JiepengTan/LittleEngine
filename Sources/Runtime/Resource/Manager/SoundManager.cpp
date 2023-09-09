/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Manager/SoundManager.h"

LittleEngine::Sound* LittleEngine::ResourceManagement::SoundManager::CreateResource(const std::string& p_path)
{
	std::string realPath = PathUtil::GetRealPath(p_path);
	LittleEngine::Sound* sound = LittleEngine::Resources::SoundLoader::Create(realPath);
	if (sound)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(sound) + offsetof(LittleEngine::Sound, path)) = p_path; // Force the resource path to fit the given path

	return sound;
}

void LittleEngine::ResourceManagement::SoundManager::DestroyResource(LittleEngine::Sound* p_resource)
{
	LittleEngine::Resources::SoundLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::SoundManager::ReloadResource(LittleEngine::Sound* p_resource, const std::string& p_path)
{
}
