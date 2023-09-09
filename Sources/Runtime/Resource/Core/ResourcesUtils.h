﻿#pragma once
#include "Core/Reflection/Reflection.h"
#include "Resource/Data/ResIncludeBasic.h"


namespace LittleEngine
{

    class ShaderResPtr;
    class TextureResPtr;
    class ModelResPtr;
    class AnimationResPtr;
    class SoundResPtr;
    class MaterialResPtr;

    
    class ResourcesUtils
    {
    private:
        static void DoNothing();
    public:
        
    #define DELCARE_RES_LOAD_FUNCTION(restypename)\
        static restypename* Load##restypename(StringText guid);\
        static restypename##ResPtr Load##restypename##ResPtr(StringText guid);
        
        static Shader* LoadShader(std::string guid);
        static ShaderResPtr LoadShaderResPtr(std::string guid);
        
        //DELCARE_RES_LOAD_FUNCTION(Shader)
        DELCARE_RES_LOAD_FUNCTION(Texture)
        DELCARE_RES_LOAD_FUNCTION(Model)
        DELCARE_RES_LOAD_FUNCTION(Animation)
        DELCARE_RES_LOAD_FUNCTION(Sound)
        DELCARE_RES_LOAD_FUNCTION(Material)
        
    };
}