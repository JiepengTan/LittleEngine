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
        static void* CreateInstance(TypeID typeId){ return Reflection::TypeInfo::CreateInstance(typeId);}
        static void* CreateInstance(std::string typeName){ return Reflection::TypeInfo::CreateInstance(typeName);}
        
        template<typename  T>
        static bool IsSubclassOf(TypeID typeId);
    };
  
    template <typename T>
    bool TypeUtil::IsSubclassOf(TypeID typeId)
    {
        auto type = GetType(typeId);
        if(type == nullptr) return false;
        return type->IsSubclassOf(T::GetStaticTypeID());
    }
}
