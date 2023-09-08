#include "StringUtil.h"

#include <algorithm>
#include <iostream>

static int parse_flag = 0;

namespace LittleEngine
{
    void fatalError(const std::string& error)
    {
        std::cerr << "Error: " << error << std::endl;

        exit(EXIT_FAILURE);
    }

    std::vector<std::string> split(std::string input, std::string pat)
    {
        std::vector<std::string> ret_list;
        while (true)
        {
            size_t index = input.find(pat);
            std::string sub_list = input.substr(0, index);
            if (!sub_list.empty())
            {
                ret_list.push_back(sub_list);
            }
            input.erase(0, index + pat.size());
            if (index == -1)
            {
                break;
            }
        }
        return ret_list;
    }


    std::string StringUtil::GetStringWithoutQuot(std::string input)
    {
        size_t left = input.find_first_of('\"') + 1;
        size_t right = input.find_last_of('\"');
        if (left > 0 && right < input.size() && left < right)
        {
            return input.substr(left, right - left);
        }
        else
        {
            return input;
        }
    }

    std::string StringUtil::Trim(const std::string& context)
    {
        std::string ret_string = context;

        ret_string = ret_string.erase(0, ret_string.find_first_not_of(" "));
        ret_string = ret_string.erase(ret_string.find_last_not_of(" ") + 1);

        return ret_string;
    }

    bool StringUtil::StartWith(const std::string& longStr, const std::string& shortStr)
    {
        if (longStr.size() < shortStr.size()) return false;
        for (int index = 0; index < shortStr.size(); ++index)
        {
            if (longStr[index] != shortStr[index]) return false;
        }
        return true;
    }

    std::string StringUtil::Replace(std::string& source_string, std::string sub_string, const std::string new_string)
    {
        std::string::size_type pos = 0;
        while ((pos = source_string.find(sub_string)) != std::string::npos)
        {
            source_string.replace(pos, sub_string.length(), new_string);
        }
        return source_string;
    }

    std::string StringUtil::Replace(std::string& source_string, char taget_char, const char new_char)
    {
        std::replace(source_string.begin(), source_string.end(), taget_char, new_char);
        return source_string;
    }

    std::string StringUtil::ToUpper(std::string& source_string)
    {
        transform(source_string.begin(), source_string.end(), source_string.begin(), ::toupper);
        return source_string;
    }

    std::string StringUtil::Join(std::vector<std::string> context_list, std::string separator)
    {
        std::string ret_string;
        if (context_list.size() == 0)
        {
            return ret_string;
        }
        ret_string = context_list[0];
        for (int index = 1; index < context_list.size(); ++index)
        {
            ret_string += separator + context_list[index];
        }

        return ret_string;
    }

    std::string StringUtil::Trim(std::string& source_string, const std::string trim_chars)
    {
        size_t left_pos = source_string.find_first_not_of(trim_chars);
        if (left_pos == std::string::npos)
        {
            source_string = std::string();
        }
        else
        {
            size_t right_pos = source_string.find_last_not_of(trim_chars);
            source_string = source_string.substr(left_pos, right_pos - left_pos + 1);
        }
        return source_string;
    }
} // namespace Utils
