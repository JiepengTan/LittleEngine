#pragma once
#include "Core/CoreInclude.h"
#include "Core/Reflection/Reflection.h"
#ifdef false
namespace OvCore
{
    // 资源的内部数据，没有文件映射关系
    REFLECTION_TYPE(ResData)
    CLASS(ResData, Fields)
    {
        REFLECTION_BODY(ResData)
    public:
        int64_t objectId;
        StringName name;
    };

    // resource object, There are specific file types that correspond to it
    REFLECTION_TYPE(ResObject)
    CLASS(ResObject, Fields)
    {
        REFLECTION_BODY(ResObject)
    public:
        OvCore::Guid guid;
        StringName name;
    };


}
#endif