#pragma once
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
        static std::string GetTypeNameWithoutNamespace(TypeID typeId);
        static TypeID GetTypeID(std::string typeName);
        static bool IsAbstract(TypeID typeId){ return Reflection::TypeInfo::IsAbstract(typeId);}
        static bool IsAbstract(std::string typeName){ return Reflection::TypeInfo::IsAbstract(typeName);}
        static bool IsEnum(TypeID typeId){ return Reflection::TypeInfo::IsEnum(typeId);}
        static bool IsEnum(std::string typeName){ return Reflection::TypeInfo::IsEnum(typeName);}
        
        static void* CreateInstance(TypeID typeId){ return Reflection::TypeInfo::CreateInstance(typeId);}
        static void* CreateInstance(std::string typeName){ return Reflection::TypeInfo::CreateInstance(typeName);}

        
        template<typename  T>
        static bool IsSubclassOf(TypeID typeId);


        // enum
        static std::vector<std::string> GetEnumNameVector(TypeID typeId);
        static std::vector<int64_t> GetEnumValueVector(TypeID typeId);
        static std::vector<std::string> GetEnumNameVector(const std::string& typeName);
        static std::vector<int64_t> GetEnumValueVector(const std::string& typeName);
        static void EnumFromString(TypeID typeId, const std::string& strValue,void* instance);
        static std::string EnumToString(TypeID typeId, void* instance);
        static void EnumFromString(const std::string& typeName, const std::string& strValue,void* instance);
        static std::string EnumToString(const std::string& typeName, void* instance);
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

    
    inline std::vector<std::string> TypeUtil::GetEnumNameVector(TypeID typeId)
    {
        return Reflection::TypeInfo::GetEnumNameVector(typeId);
    }

    inline std::vector<int64_t> TypeUtil::GetEnumValueVector(TypeID typeId)
    {
        return Reflection::TypeInfo::GetEnumValueVector(typeId);
    }

    inline std::vector<std::string> TypeUtil::GetEnumNameVector(const std::string& typeName)
    {
        return Reflection::TypeInfo::GetEnumNameVector(typeName);
    }

    inline std::vector<int64_t> TypeUtil::GetEnumValueVector(const std::string& typeName)
    {
        return Reflection::TypeInfo::GetEnumValueVector(typeName);
    }

    inline void TypeUtil::EnumFromString(TypeID typeId, const std::string& strValue, void* instance)
    {
        Reflection::TypeInfo::EnumFromString(typeId,strValue,instance);
    }

    inline std::string TypeUtil::EnumToString(TypeID typeId, void* instance)
    {
        return Reflection::TypeInfo::EnumToString(typeId,instance);
    }

    inline void TypeUtil::EnumFromString(const std::string& typeName, const std::string& strValue, void* instance)
    {
        Reflection::TypeInfo::EnumFromString(typeName,strValue,instance);
    }

    inline std::string TypeUtil::EnumToString(const std::string& typeName, void* instance)
    {
        return Reflection::TypeInfo::EnumToString(typeName,instance);
    }
}
