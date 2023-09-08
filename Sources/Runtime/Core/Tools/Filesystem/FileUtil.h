#pragma once
#include <string>
#include <vector>

namespace LittleEngine
{
    class FileUtil
    {
    public:
        static std::string ReadAllText(const std::string& filePath);
        static std::vector<std::string> ReadAllLines(const std::string& filePath);
        static std::vector<unsigned char> ReadAllBytes(const std::string& filePath);
        static void WriteAllText(const std::string& filePath, const std::string& text);
        static void WriteAllLines(const std::string& filePath, const std::vector<std::string>& lines);
        static void WriteAllBytes(const std::string& filePath, const std::vector<unsigned char>& bytes);
    };
}
