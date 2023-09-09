#pragma once
#include <string>

namespace LittleEngine
{
    class TextAsset
    {
        std::string GetText(){ return text;}
    public:
        std::string text;
        std::string path;
    };
}
