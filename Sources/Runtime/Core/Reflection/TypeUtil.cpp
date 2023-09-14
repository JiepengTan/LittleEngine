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


    //void TypeUtil::SetField(void* instance, Reflection::FieldAccessor& field, int& val)
    //{
    //    LE_ASSERT(field.GetFieldTypeName() == "int","error field type");
    //    field.Get(instance,val);
    //}
//
//
    //void TypeUtil::SetField(void* instance, Reflection::FieldAccessor& field, std::string& val)
    //{
    //    LE_ASSERT(field.GetFieldTypeName() == "std::string","error field type");
    //    field.Set(instance,&val);
    //}

    std::string TypeUtil::GetTypeName(TypeID typeId)
    {
        if(!HasType(typeId))
            return "";
        return GetType(typeId)->GetTypeName();
    }

    std::string TypeUtil::GetTypeNameWithoutNamespace(TypeID typeId)
    {
        return Reflection::MetaRegisterUtil::GetTypeNameWithoutNamespace(typeId);
    }

    TypeID TypeUtil::GetTypeID(std::string typeName)
    {
        if(!HasType(typeName))
            return k_invalidTypeID;
        return GetType(typeName)->GetTypeID();
    }
    
    std::vector<std::string> TypeUtil::GetEnumNameVector(TypeID typeId)
    {
        return Reflection::TypeInfo::GetEnumNameVector(typeId);
    }

    std::vector<int64_t> TypeUtil::GetEnumValueVector(TypeID typeId)
    {
        return Reflection::TypeInfo::GetEnumValueVector(typeId);
    }

    std::vector<std::string> TypeUtil::GetEnumNameVector(const std::string& typeName)
    {
        return Reflection::TypeInfo::GetEnumNameVector(typeName);
    }

    std::vector<int64_t> TypeUtil::GetEnumValueVector(const std::string& typeName)
    {
        return Reflection::TypeInfo::GetEnumValueVector(typeName);
    }

    void TypeUtil::EnumFromString(TypeID typeId, const std::string& strValue, void* instance)
    {
        Reflection::TypeInfo::EnumFromString(typeId,strValue,instance);
    }

    std::string TypeUtil::EnumToString(TypeID typeId, void* instance)
    {
        return Reflection::TypeInfo::EnumToString(typeId,instance);
    }

    void TypeUtil::EnumFromString(const std::string& typeName, const std::string& strValue, void* instance)
    {
        Reflection::TypeInfo::EnumFromString(typeName,strValue,instance);
    }

    std::string TypeUtil::EnumToString(const std::string& typeName, void* instance)
    {
        return Reflection::TypeInfo::EnumToString(typeName,instance);
    }
    
    int64_t TypeUtil::GetEnumValue(void* p_instance, Reflection::FieldAccessor p_field)
    {
        _ASSERT(p_field.IsEnum(),"shoud call GetEnumValue in a enum field");
        return *(int64_t*)p_field.Get(p_instance);
    }

    void TypeUtil::SetEnumValue(void* p_instance, Reflection::FieldAccessor p_field, int64_t p_value)
    {
        _ASSERT(p_field.IsEnum(),"shoud call SetEnumValue in a enum field");
        return p_field.Set(p_instance,(int64_t*) p_value);
    }
}

