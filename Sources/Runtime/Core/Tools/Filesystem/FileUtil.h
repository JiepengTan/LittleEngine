#pragma once
#include <string>
#include <vector>

namespace LittleEngine
{
    class FileUtil
    {
    public:
        static std::string GetRealPath(const std::string& p_path);
        static void ProvideAssetPaths(const std::string & p_projectAssetsPath, const std::string & p_engineAssetsPath);
        static std::string ReadAllText(const std::string& filePath);
        static std::vector<std::string> ReadAllLines(const std::string& filePath);
        static std::vector<unsigned char> ReadAllBytes(const std::string& filePath);
        static void WriteAllText(const std::string& filePath, const std::string& text);
        static void WriteAllLines(const std::string& filePath, const std::vector<std::string>& lines);
        static void WriteAllBytes(const std::string& filePath, const std::vector<unsigned char>& bytes);

    private:
        inline static std::string __PROJECT_ASSETS_PATH = "";
        inline static std::string __ENGINE_ASSETS_PATH = "";
    };
}
