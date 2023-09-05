/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/ResourceManagement/ShaderManager.h"

LittleEngine::Rendering::Resources::Shader* LittleEngine::ResourceManagement::ShaderManager::CreateResource(const std::string & p_path)
{
	std::string realPath = GetRealPath(p_path);
	LittleEngine::Rendering::Resources::Shader* shader = LittleEngine::Rendering::Resources::Loaders::ShaderLoader::Create(realPath);
	if (shader)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(LittleEngine::Rendering::Resources::Shader, path)) = p_path; // Force the resource path to fit the given path

	return shader;
}

void LittleEngine::ResourceManagement::ShaderManager::DestroyResource(LittleEngine::Rendering::Resources::Shader* p_resource)
{
	LittleEngine::Rendering::Resources::Loaders::ShaderLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::ShaderManager::ReloadResource(LittleEngine::Rendering::Resources::Shader* p_resource, const std::string& p_path)
{
	LittleEngine::Rendering::Resources::Loaders::ShaderLoader::Recompile(*p_resource, p_path);
}
