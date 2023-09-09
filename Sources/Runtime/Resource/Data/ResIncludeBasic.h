#pragma once
#include "Core/CoreInclude.h"
#include "Core/Reflection/Reflection.h"

namespace LittleEngine
{
    class Shader;
    class Texture;
    class Model;
    class Animation;
    class Sound;
    class Material;
    
    class IMesh;
    class Mesh;
    
    // 资源的内部数据，没有文件映射关系
    REFLECTION_TYPE(ResData)
    CLASS(ResData, Fields)
    {
        REFLECTION_BODY(ResData)
    };

    // resource object, There are specific file types that correspond to it
    REFLECTION_TYPE(ResObject)
    CLASS(ResObject, Fields)
    {
        REFLECTION_BODY(ResObject)
    public:
        StringName m_guid;
        StringName m_name;
    };
}
