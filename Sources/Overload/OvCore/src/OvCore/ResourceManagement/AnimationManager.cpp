/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/ResourceManagement/AnimationManager.h"
#include <OvTools/Filesystem/IniFile.h>

#include "OvDebug/Logger.h"
#include "OvRendering/Resources/Loaders/AnimationLoader.h"



OvRendering::Resources::Animation* OvCore::ResourceManagement::AnimationManager::CreateResource(const std::string& p_path)
{
	std::string realPath = GetRealPath(p_path);
	auto anim = OvRendering::Resources::Loaders::AnimationLoader::Create( realPath,currentModel);
	if(anim != nullptr)
	{
		anim->m_path = p_path; // Force the resource path to fit the given path
	}
	return anim;
}

void OvCore::ResourceManagement::AnimationManager::DestroyResource(OvRendering::Resources::Animation* p_resource)
{
	OvRendering::Resources::Loaders::AnimationLoader::Destroy(p_resource);
}

void OvCore::ResourceManagement::AnimationManager::ReloadResource(OvRendering::Resources::Animation* p_resource, const std::string& p_path)
{
	OVLOG_ERROR("Now do not support reload animation!");
	return;
	std::string realPath = GetRealPath(p_path);
	OvRendering::Resources::Loaders::AnimationLoader::Reload(*p_resource, realPath, currentModel);
}
