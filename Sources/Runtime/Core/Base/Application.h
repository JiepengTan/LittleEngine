#pragma once
#include "Core/Maths/FFloat.h"
#include "Core/String/StringName.h"

namespace LittleEngine
{
    struct Application
    {
    public:
        static bool isEditorMode;
        static bool isPlaying;
        static StringName version;
        static StringName platformName;
    };
};
