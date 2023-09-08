/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Filesystem/IniFile.h"
#include "Core/CoreInclude.h"
#include "TextAssetManager.h"

#include <fstream>

#include "Core/Tools/Filesystem/FileUtil.h"

namespace LittleEngine::ResourceManagement
{
    TextAsset* LittleEngine::ResourceManagement::TextAssetManager::CreateResource(
        const std::string& p_path)
    {
        std::string realPath = GetRealPath(p_path);
        auto* asset = new TextAsset();
        asset->path = p_path;
        asset->text = FileUtil::ReadAllText(realPath);
        return asset;
    }

    void LittleEngine::ResourceManagement::TextAssetManager::DestroyResource(
        TextAsset* p_resource)
    {
        delete p_resource;
    }

    void LittleEngine::ResourceManagement::TextAssetManager::ReloadResource(
        TextAsset* p_resource, const std::string& p_path)
    {
        std::string realPath = GetRealPath(p_path);
        p_resource->path = p_path;
        p_resource->text = FileUtil::ReadAllText(realPath);
    }
}
