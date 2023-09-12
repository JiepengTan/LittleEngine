﻿#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/Base/Macro.h"

namespace LittleEngine
{
    typedef Reflection::TypeInfo* TypeInfoPtr;
    class TypeUtil
    {
    public:
        static void LoadAllTypeInfo();
        static void UnloadAllTypeInfo();
        
    public:
        static TMap<TypeID, TVector<TypeInfoPtr> > GetBaseClassInfos();
        static TVector<TypeInfoPtr>  GetAllTypes();
        static TypeInfoPtr GetType(TypeID typeId);
        static TypeInfoPtr GetType(std::string typeName);
        template<typename T>
        static T* GetField(void* instance, Reflection::FieldAccessor& field);
        template<typename T>
        static void SetField(void* instance, Reflection::FieldAccessor& field, T* val);

        
        static bool HasType(TypeID typeId) { return GetType(typeId) != nullptr; }
        static bool HasType(std::string typeName) {  return GetType(typeName) != nullptr; }
        static std::string GetTypeName(TypeID typeId);
        static TypeID GetTypeID(std::string typeName);
        static bool IsAbstract(TypeID typeId){ return Reflection::TypeInfo::IsAbstract(typeId);}
        static bool IsAbstract(std::string typeName){ return Reflection::TypeInfo::IsAbstract(typeName);}
        
        static void* CreateInstance(TypeID typeId){ return Reflection::TypeInfo::CreateInstance(typeId);}
        static void* CreateInstance(std::string typeName){ return Reflection::TypeInfo::CreateInstance(typeName);}
        
        template<typename  T>
        static bool IsSubclassOf(TypeID typeId);
    };

    template <typename T>
    T* TypeUtil::GetField(void* instance, Reflection::FieldAccessor& field)
    {
        return static_cast<T*>(field.Get(instance));
    }

    template <typename T>
    void TypeUtil::SetField(void* instance, Reflection::FieldAccessor& field, T* val)
    {
        field.Set(instance,val);
    }

    template <typename T>
    bool TypeUtil::IsSubclassOf(TypeID typeId)
    {
        auto type = GetType(typeId);
        if(type == nullptr) return false;
        return type->IsSubclassOf(T::GetStaticTypeID());
    }
}
