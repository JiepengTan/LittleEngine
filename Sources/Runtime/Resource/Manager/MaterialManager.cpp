/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/ResourceManagement/MaterialManager.h"

LittleEngine::Resources::Material * LittleEngine::ResourceManagement::MaterialManager::CreateResource(const std::string & p_path)
{
	std::string realPath = PathUtil::GetRealPath(p_path);

	Resources::Material* material = LittleEngine::Resources::Loaders::MaterialLoader::Create(realPath);
	if (material)
	{
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(material) + offsetof(Resources::Material, path)) = p_path; // Force the resource path to fit the given path
	}

	return material;
}

void LittleEngine::ResourceManagement::MaterialManager::DestroyResource(LittleEngine::Resources::Material * p_resource)
{
	LittleEngine::Resources::Loaders::MaterialLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::MaterialManager::ReloadResource(LittleEngine::Resources::Material* p_resource, const std::string& p_path)
{
	LittleEngine::Resources::Loaders::MaterialLoader::Reload(*p_resource, p_path);
}
