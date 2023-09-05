/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/ResourceManagement/AnimationManager.h"
#include "Core/Tools/Filesystem/IniFile.h"

#include "Core/CoreInclude.h"
#include "Modules/Rendering/Resources/Loaders/AnimationLoader.h"



LittleEngine::Rendering::Resources::Animation* LittleEngine::ResourceManagement::AnimationManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);
	auto anim = LittleEngine::Rendering::Resources::Loaders::AnimationLoader::Create( realPath,currentModel);
	if(anim != nullptr)
	{
		anim->path = p_path; // Force the resource path to fit the given path
	}
	return anim;
}

void LittleEngine::ResourceManagement::AnimationManager::DestroyResource(LittleEngine::Rendering::Resources::Animation* p_resource)
{
	LittleEngine::Rendering::Resources::Loaders::AnimationLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::AnimationManager::ReloadResource(LittleEngine::Rendering::Resources::Animation* p_resource, const std::string& p_path)
{
	OVLOG_ERROR("Now do not support reload animation!");
	return;
	std::string realPath = GetRealPath(p_path);
	LittleEngine::Rendering::Resources::Loaders::AnimationLoader::Reload(*p_resource, realPath, currentModel);
}
