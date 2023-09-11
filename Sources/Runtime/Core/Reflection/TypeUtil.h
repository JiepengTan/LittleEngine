#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/Base/Macro.h"

namespace LittleEngine
{
    class TypeUtil
    {
    public:
        static void LoadAllTypeInfo();
        static void UnloadAllTypeInfo();
        
    public:
        static TMap<TypeID, TVector<Reflection::TypeInfo*> > GetBaseClassInfos();
        static TVector<Reflection::TypeInfo*>  GetAllTypes();
    };
}
