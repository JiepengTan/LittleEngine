/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Manager/ShaderManager.h"

LittleEngine::Shader* LittleEngine::ResourceManagement::ShaderManager::CreateResource(const std::string & p_path)
{
	std::string realPath = PathUtil::GetRealPath(p_path);
	LittleEngine::Shader* shader = LittleEngine::Resources::ShaderLoader::Create(realPath);
	if (shader)
		*reinterpret_cast<std::string*>(reinterpret_cast<char*>(shader) + offsetof(LittleEngine::Shader, path)) = p_path; // Force the resource path to fit the given path

	return shader;
}

void LittleEngine::ResourceManagement::ShaderManager::DestroyResource(LittleEngine::Shader* p_resource)
{
	LittleEngine::Resources::ShaderLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::ShaderManager::ReloadResource(LittleEngine::Shader* p_resource, const std::string& p_path)
{
	LittleEngine::Resources::ShaderLoader::Recompile(*p_resource, p_path);
}
