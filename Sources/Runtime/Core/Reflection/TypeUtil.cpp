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

    TMap<TypeID, TVector<TypeInfoPtr> > TypeUtil::GetBaseClassInfos()
    {
        return Reflection::TypeInfo::GetBaseClassInfos();
    }

    TVector<TypeInfoPtr> TypeUtil::GetAllTypes()
    {
        return Reflection::TypeInfo::GetAllTypes();
    }

    TypeInfoPtr TypeUtil::GetType(TypeID typeId)
    {
        return Reflection::TypeInfo::GetType(typeId);
    }
    TypeInfoPtr TypeUtil::GetType(std::string typeName)
    {
        return Reflection::TypeInfo::GetType(typeName);
    }
  
}

