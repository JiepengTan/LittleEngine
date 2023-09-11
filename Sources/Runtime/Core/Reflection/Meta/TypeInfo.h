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

        static bool               NewArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor);
        static void*              CreateFromNameAndJson(std::string type_name, const Json& json_context);
        static Json               WriteByName(std::string type_name, void* instance);

        std::string GetTypeName();
        TypeID GetTypeID() const;

        int GetFieldsList(FieldAccessor*& out_list);
        int GetMethodsList(MethodAccessor*& out_list);


        FieldAccessor GetFieldByName(const char* name);
        MethodAccessor GetMethodByName(const char* name);
        bool IsSubclassOf(TypeID typeId);
        bool IsAssignableFrom(TypeID typeId);
        bool IsValid() { return m_isValid; }

        TypeInfo& operator=(const TypeInfo& dest);

        static TypeInfo* GetType(std::string type_name);
        static TypeInfo* GetType(TypeID typeId);
        static TypeInfo* RegisterType(std::string type_name,TypeID typeId);
        void*  CreateInstance();
        static void* CreateInstance(TypeID typeId);
        static void* CreateInstance(std::string type_name);
        
        static TMap<TypeID,TVector<TypeInfo*> > GetBaseClassInfos();
        static TVector<TypeInfo*>  GetAllTypes();

    private:
        TypeInfo(std::string type_name,TypeID typeId);
        static TypeInfo NewMetaFromName(std::string type_name);
        static std::vector<TypeID> GetBaseClassIds(std::string type_name);
        static void RecvGetSuperClassMetas(std::string type_name,std::vector<TypeInfo*>& supperClasses);
       
        static void Clear();
        static std::map<TypeID,TypeInfo*> m_id2Types;
        static std::map<std::string,TypeInfo*> m_name2Types;
        static TVector<TypeInfo*>  m_allTypes;
        static std::map<TypeID,std::vector<TypeInfo*>> m_id2BaseClassTypes;
    private:

        std::vector<FieldAccessor>   m_fields;
        std::vector<MethodAccessor> m_methods;
        std::string                                                 m_typeName;

        bool m_isValid;
        TypeID m_typeId;
    };
}
