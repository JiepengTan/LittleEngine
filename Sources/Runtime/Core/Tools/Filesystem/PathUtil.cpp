#include "PathUtil.h"

#include <filesystem>
#include <regex>
#include <system_error>

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
    void PathUtil::WalkFileName(std::string strPath, std::string ext, const std::function<void(const std::string&)>& func)
    {
        Walk(strPath,ext,func,true);
    }
    void PathUtil::Walk(std::string strPath, std::string ext, const std::function<void(const std::string&)>& func ,bool isFileName )
    {
        if(ext.empty())
        {
            ext = ".*";
        }
        std::vector<std::string> matches;
        std::filesystem::path fsPath(strPath);
        std::regex regEx(ext,std::regex_constants::extended);
        std::filesystem::recursive_directory_iterator endIterator;
        for (std::filesystem::recursive_directory_iterator it(fsPath); it != endIterator; ++it) {
            auto fileName = it->path().filename().string();
            if ( std::regex_match(fileName, regEx)) {
                if(isFileName)
                {
                    matches.push_back(fileName);
                }
                else
                {
                    matches.push_back(it->path().string());
                }
            }
        }
        
        for (auto path : matches)
        {
            func(path);
        }
    }
}
