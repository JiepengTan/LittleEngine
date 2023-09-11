/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Manager/MaterialManager.h"

LittleEngine::Material * LittleEngine::ResourceManagement::MaterialManager::CreateResource(const std::string & p_path)
{
	std::string realPath = PathUtil::GetRealPath(p_path);

	Material* material = LittleEngine::Resources::MaterialLoader::Create(realPath);
	if (material)
	{
		material->path = p_path; // Force the resource path to fit the given path
	}

	return material;
}

void LittleEngine::ResourceManagement::MaterialManager::DestroyResource(LittleEngine::Material * p_resource)
{
	LittleEngine::Resources::MaterialLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::MaterialManager::ReloadResource(LittleEngine::Material* p_resource, const std::string& p_path)
{
	LittleEngine::Resources::MaterialLoader::Reload(*p_resource, p_path);
}
