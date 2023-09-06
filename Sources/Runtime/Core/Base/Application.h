#pragma once
#include <memory>

#include "Core/Base/Macro.h"
#include "Core/Maths/FFloat.h"

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
