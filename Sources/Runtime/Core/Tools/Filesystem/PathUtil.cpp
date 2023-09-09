#include "PathUtil.h"

#include "Core/Tools/Utils/StringUtil.h"

namespace LittleEngine
{
    std::string PathUtil::GetRealPath(const std::string& p_path)
    {
        std::string result;

        if (p_path[0] == ':') // The path is an engine path
        {
            result = __ENGINE_ASSETS_PATH + std::string(p_path.data() + 1, p_path.data() + p_path.size());
        }
        else if (!StringUtil::StartWith(p_path, __PROJECT_ASSETS_PATH)) // The path is a project path
        {
            result = __PROJECT_ASSETS_PATH + p_path;
        }
        else // The path is a project path
        {
            result = p_path;
        }

        return result;
    }

    void PathUtil::ProvideAssetPaths(const std::string& p_projectAssetsPath, const std::string& p_engineAssetsPath)
    {
        __PROJECT_ASSETS_PATH = p_projectAssetsPath;
        __ENGINE_ASSETS_PATH = p_engineAssetsPath;
    }
}
