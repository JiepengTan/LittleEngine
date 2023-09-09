#pragma once
#include "Core/Reflection/Reflection.h"
#include "Resource/Data/ResIncludeBasic.h"

namespace LittleEngine
{
    class ResourcesUtils
    {
    public:
        static Shader* LoadShader(StringText guid);
        static Texture* LoadTexture(StringText guid);
        static Model* LoadModel(StringText guid);
        static Animation* LoadAnimation(StringText guid);
        static Sound* LoadSound(StringText guid);
        static Material* LoadMaterial(StringText guid);
    };
}
