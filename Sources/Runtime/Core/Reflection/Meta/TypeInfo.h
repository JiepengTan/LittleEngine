#pragma once
#include "MetaAccessor.h"
#include "MetaMacro.h"

namespace LittleEngine::Reflection
{
    class TypeInfo
    {
        friend class FieldAccessor;
        friend class ArrayAccessor;
        friend class MetaRegisterUtil;

    public:
        TypeInfo();

        // static void Register();

        static bool               NewArrayAccessorFromName(std::string arrayTypeName, ArrayAccessor& accessor);
        static void*              CreateFromNameAndJson(std::string typeName, const Json& jsonContext);
        static Json               WriteByName(std::string typeName, void* instance);

        std::string GetTypeName();
        TypeID GetTypeID() const;

        int GetFieldsList(FieldAccessor*& outList);
        int GetMethodsList(MethodAccessor*& outList);


        bool HasMeta(std::string metaKey);
        std::string GetMeta(std::string metaKey);
        
        
        FieldAccessor* GetFieldByName(std::string name);
        MethodAccessor* GetMethodByName(std::string name);
        bool IsSubclassOf(TypeID typeId);
        bool IsAssignableFrom(TypeID typeId);
        bool IsValid() { return m_isValid; }
        template<class T>
        bool IsAssignableFrom(){
            return IsAssignableFrom(T::GetStaticTypeID());
        }
        bool IsAbstract();
        bool IsEnum();

        TypeInfo& operator=(const TypeInfo& dest);

        static TypeInfo* GetType(std::string typeName);
        static TypeInfo* GetType(TypeID typeId);
        static bool HasType(TypeID typeId);
        static bool HasType(std::string typeName);
        static bool IsAbstract(TypeID typeId);
        static bool IsAbstract(std::string typeName);
        static bool IsEnum(TypeID typeId);
        static bool IsEnum(std::string typeName);
        
        static TypeInfo* RegisterType(std::string typeName,TypeID typeId);
        void*  CreateInstance();
        static void* CreateInstance(TypeID typeId);
        static void* CreateInstance(std::string typeName);

        static std::vector<std::string> GetEnumNameVector(TypeID typeId);
        static std::vector<int64_t> GetEnumValueVector(TypeID typeId);
        static std::vector<std::string> GetEnumNameVector(const std::string& typeName);
        static std::vector<int64_t> GetEnumValueVector(const std::string& typeName);
        static void EnumFromString(TypeID typeId, const std::string& strValue,void* instance);
        static std::string EnumToString(TypeID typeId, void* instance);
        static void EnumFromString(const std::string& typeName, const std::string& strValue,void* instance);
        static std::string EnumToString(const std::string& typeName, void* instance);
        
        
        static TMap<TypeID,TVector<TypeInfo*> > GetBaseClassInfos();
        static TVector<TypeInfo*>  GetAllTypes();

        std::vector<FieldAccessor>& GetFields(){ return m_fields;}
        std::vector<MethodAccessor>& GetMethods(){ return m_methods;}
    private:
        TypeInfo(std::string typeName,TypeID typeId);
        static TypeInfo NewMetaFromName(std::string typeName);
        static std::vector<TypeID> GetBaseClassIds(std::string typeName);
        static void RecvGetSuperClassMetas(std::string typeName,std::vector<TypeInfo*>& supperClasses);
       
        static void Clear();
        static std::map<TypeID,TypeInfo*> m_id2Types;
        static std::map<std::string,TypeInfo*> m_name2Types;
        static TVector<TypeInfo*>  m_allTypes;
        static std::map<TypeID,std::vector<TypeInfo*>> m_id2BaseClassTypes;
    private:

        std::vector<FieldAccessor>   m_fields;
        std::vector<MethodAccessor> m_methods;
        std::string                                                 m_typeName;
        std::map<std::string,std::string> m_metaData;
        bool m_isValid;
        TypeID m_typeId;
    };
}
