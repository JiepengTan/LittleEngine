/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Manager/TextureManager.h"
#include "Modules/Rendering/Settings/DriverSettings.h"

#include "Core/Tools/Filesystem/IniFile.h"

std::tuple<LittleEngine::Rendering::Settings::ETextureFilteringMode, LittleEngine::Rendering::Settings::ETextureFilteringMode, bool> GetAssetMetadata(const std::string& p_path)
{
	auto metaFile = LittleEngine::Filesystem::IniFile(p_path + ".meta");

	auto min = metaFile.GetOrDefault("MIN_FILTER", static_cast<int>(LittleEngine::Rendering::Settings::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	auto mag = metaFile.GetOrDefault("MAG_FILTER", static_cast<int>(LittleEngine::Rendering::Settings::ETextureFilteringMode::LINEAR));
	auto mipmap = metaFile.GetOrDefault("ENABLE_MIPMAPPING", true);

	return { static_cast<LittleEngine::Rendering::Settings::ETextureFilteringMode>(min), static_cast<LittleEngine::Rendering::Settings::ETextureFilteringMode>(mag), mipmap };
}

LittleEngine::Texture* LittleEngine::ResourceManagement::TextureManager::CreateResource(const std::string & p_path)
{
	std::string realPath = PathUtil::GetRealPath(p_path);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	LittleEngine::Texture* texture = LittleEngine::Resources::TextureLoader::Create(realPath, min, mag, mipmap);
	if (texture)
		texture->path = p_path; // Force the resource path to fit the given path

	return texture;
}

void LittleEngine::ResourceManagement::TextureManager::DestroyResource(LittleEngine::Texture* p_resource)
{
	LittleEngine::Resources::TextureLoader::Destroy(p_resource);
}

void LittleEngine::ResourceManagement::TextureManager::ReloadResource(LittleEngine::Texture* p_resource, const std::string& p_path)
{
	std::string realPath = PathUtil::GetRealPath(p_path);

	auto [min, mag, mipmap] = GetAssetMetadata(realPath);

	LittleEngine::Resources::TextureLoader::Reload(*p_resource, realPath, min, mag, mipmap);
}
