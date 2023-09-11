#include "TypeUtil.h"

#include "Core/Reflection/Reflection.h"

namespace LittleEngine
{
    void TypeUtil::LoadAllTypeInfo()
    {
        Reflection::MetaRegisterUtil::RegisterAll();
    }
    void TypeUtil::UnloadAllTypeInfo()
    {
        Reflection::MetaRegisterUtil::UnRegisterAll();
    }

    TMap<TypeID, TVector<Reflection::TypeInfo*> > TypeUtil::GetBaseClassInfos()
    {
        return Reflection::TypeInfo::GetBaseClassInfos();
    }

    TVector<Reflection::TypeInfo*> TypeUtil::GetAllTypes()
    {
        return Reflection::TypeInfo::GetAllTypes();
    }
}

