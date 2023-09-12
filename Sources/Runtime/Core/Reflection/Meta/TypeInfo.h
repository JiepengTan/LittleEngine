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
        
        
        FieldAccessor GetFieldByName(const char* name);
        MethodAccessor GetMethodByName(const char* name);
        bool IsSubclassOf(TypeID typeId);
        bool IsAssignableFrom(TypeID typeId);
        bool IsValid() { return m_isValid; }
        template<class T>
        bool IsAssignableFrom(){
            return IsAssignableFrom(T::GetStaticTypeID());
        }
        bool IsAbstract();

        TypeInfo& operator=(const TypeInfo& dest);

        static TypeInfo* GetType(std::string typeName);
        static TypeInfo* GetType(TypeID typeId);
        static bool HasType(TypeID typeId);
        static bool HasType(std::string typeName);
        static bool IsAbstract(TypeID typeId);
        static bool IsAbstract(std::string typeName);
        static TypeInfo* RegisterType(std::string typeName,TypeID typeId);
        void*  CreateInstance();
        static void* CreateInstance(TypeID typeId);
        static void* CreateInstance(std::string typeName);
        
        
        static TMap<TypeID,TVector<TypeInfo*> > GetBaseClassInfos();
        static TVector<TypeInfo*>  GetAllTypes();

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
