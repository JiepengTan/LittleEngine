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
        static int64_t GetEnumValue(void* p_instance,Reflection::FieldAccessor p_field);
        static void SetEnumValue(void* p_instance,Reflection::FieldAccessor p_field,int64_t p_value);
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
