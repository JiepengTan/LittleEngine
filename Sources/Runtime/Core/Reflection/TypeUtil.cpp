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

    std::string TypeUtil::GetTypeName(TypeID typeId)
    {
        if(!HasType(typeId))
            return "";
        return GetType(typeId)->GetTypeName();
    }

    TypeID TypeUtil::GetTypeID(std::string typeName)
    {
        if(!HasType(typeName))
            return k_invalidTypeID;
        return GetType(typeName)->GetTypeID();
    }
}

