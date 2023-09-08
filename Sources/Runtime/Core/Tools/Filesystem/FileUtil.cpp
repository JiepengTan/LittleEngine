#include "FileUtil.h"

#include <fstream>
#include <stdexcept>

#include "PathUtil.h"
#include "Core/Tools/Utils/StringUtil.h"

namespace LittleEngine
{
    std::string FileUtil::GetRealPath(const std::string& p_path)
    {
        std::string result;

        if (p_path[0] == ':') // The path is an engine path
        {
            result = __ENGINE_ASSETS_PATH + std::string(p_path.data() + 1, p_path.data() + p_path.size());
        }
        else if(!StringUtil::StartWith(p_path,__PROJECT_ASSETS_PATH))// The path is a project path
        {
            result = __PROJECT_ASSETS_PATH + p_path;
        }
        else // The path is a project path
        {
            result = p_path;
        }

        return result;
    }

    void FileUtil::ProvideAssetPaths(const std::string& p_projectAssetsPath, const std::string& p_engineAssetsPath)
    {
        __PROJECT_ASSETS_PATH	= p_projectAssetsPath;
        __ENGINE_ASSETS_PATH	= p_engineAssetsPath;
        PathUtil::ProvideAssetPaths(p_projectAssetsPath,p_engineAssetsPath);
    }

    std::string FileUtil::ReadAllText(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            std::string content((std::istreambuf_iterator<char>(file)),
                                (std::istreambuf_iterator<char>()));
            file.close();
            return content;
        }
        else
        {
            throw std::runtime_error("Unable to open file: " + filePath);
        }
    }

    std::vector<std::string> FileUtil::ReadAllLines(const std::string& filePath)
    {
        std::vector<std::string> lines;
        std::ifstream file(filePath);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                lines.push_back(line);
            }
            file.close();
        }
        else
        {
            throw std::runtime_error("Unable to open file: " + filePath);
        }
        return lines;
    }

    std::vector<unsigned char> FileUtil::ReadAllBytes(const std::string& filePath)
    {
        std::vector<unsigned char> bytes;
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (file.is_open())
        {
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            bytes.resize(static_cast<size_t>(fileSize));
            if (file.read(reinterpret_cast<char*>(bytes.data()), fileSize))
            {
                file.close();
                return bytes;
            }
            else
            {
                file.close();
                throw std::runtime_error("Failed to read file: " + filePath);
            }
        }
        else
        {
            throw std::runtime_error("Unable to open file: " + filePath);
        }
        return bytes;
    }

    void FileUtil::WriteAllText(const std::string& filePath, const std::string& text)
    {
        std::ofstream file(filePath);
        if (file.is_open())
        {
            file << text;
            file.close();
        }
        else
        {
            throw std::runtime_error("Unable to open file for writing: " + filePath);
        }
    }

    void FileUtil::WriteAllLines(const std::string& filePath, const std::vector<std::string>& lines)
    {
        std::ofstream file(filePath);
        if (file.is_open())
        {
            for (const std::string& line : lines)
            {
                file << line << std::endl;
            }
            file.close();
        }
        else
        {
            throw std::runtime_error("Unable to open file for writing: " + filePath);
        }
    }


    void FileUtil::WriteAllBytes(const std::string& filePath, const std::vector<unsigned char>& bytes)
    {
        std::ofstream file(filePath, std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
            file.close();
        }
        else
        {
            throw std::runtime_error("Unable to open file for writing: " + filePath);
        }
    }
}
