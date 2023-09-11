#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/Base/Macro.h"

namespace LittleEngine
{
    class TypeUtil
    {

    public:
        static TMap<TypeID, TVector<Reflection::TypeMeta*> > GetBaseClassInfos();
        static TVector<Reflection::TypeMeta*>  GetAllTypes();
    };
}
