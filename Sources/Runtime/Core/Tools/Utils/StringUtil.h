#pragma once
#include "string"
#include "vector"

namespace LittleEngine
{
    class StringUtil
    {
    public:
        static std::string GetStringWithoutQuot(std::string input);
        static std::string Replace(std::string& source_string, std::string sub_string, const std::string new_string);
        static std::string Replace(std::string& source_string, char taget_char, const char new_char);
        static std::string ToUpper(std::string& source_string);
        static std::string Join(std::vector<std::string> context_list, std::string separator);
        static std::string Trim(std::string& source_string, const std::string trim_chars);
        static std::string Trim(const std::string& context);
        static bool StartWith(const std::string& longStr,const std::string& shortStr);
    };
}
