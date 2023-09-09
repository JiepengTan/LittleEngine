#pragma once

#include "string"
#include "vector"

namespace LittleEngine
{
    class PathUtil
    {
    public:
        static std::string GetRealPath(const std::string& p_path);
        static void ProvideAssetPaths(const std::string & p_projectAssetsPath, const std::string & p_engineAssetsPath);

    private:
        inline static std::string __PROJECT_ASSETS_PATH = "";
        inline static std::string __ENGINE_ASSETS_PATH = "";
    };
}
