// copy from https://github.com/BoomingTech/Piccolo
#include "Core/Base/Macro.h"
#include "cstring"
#include "map"
#include "MetaAccessor.h"
#include "MetaRegisterUtil.h"
#include "TypeInfo.h"


namespace LittleEngine::Reflection
{
    std::map<TypeID, TypeInfo*> TypeInfo::m_id2Types;
    TVector<TypeInfo*> TypeInfo::m_allTypes;
    std::map<std::string, TypeInfo*> TypeInfo::m_name2Types;
    std::map<TypeID, std::vector<TypeInfo*>> TypeInfo::m_id2BaseClassTypes;

    TypeInfo* TypeInfo::GetType(std::string type_name)
    {
        if (m_name2Types.count(type_name) == 0) return nullptr;
        return m_name2Types.at(type_name);
    }

    TypeInfo* TypeInfo::GetType(TypeID typeId)
    {
        if (m_id2Types.count(typeId) == 0) return nullptr;
        return m_id2Types.at(typeId);
    }

    TVector<TypeInfo*> TypeInfo::GetAllTypes()
    {
        if (m_allTypes.size() != m_id2Types.size())
        {
            m_allTypes.clear();
            for (auto item : m_id2Types)
            {
                m_allTypes.push_back(item.second);
            }
        }
        return m_allTypes;
    }

    void TypeInfo::RecvGetSuperClassMetas(std::string type_name, std::vector<TypeInfo*>& supperClasses)
    {
        auto ids = GetBaseClassIds(type_name);
        for (auto id : ids)
        {
            if (m_id2Types.count(id) == 0)
            {
                auto className = MetaRegisterUtil::m_id2ClassNameMap.at(id);
                auto base = RegisterType(className, id);
                supperClasses.push_back(base);
            }
            else
            {
                supperClasses.push_back(m_id2Types.at(id));
            }
            RecvGetSuperClassMetas(MetaRegisterUtil::m_id2ClassNameMap.at(id), supperClasses);
        }
    }

    TypeInfo* TypeInfo::RegisterType(std::string type_name, TypeID typeId)
    {
        if (m_id2Types.count(typeId) != 0) return m_id2Types.at(typeId);
        TypeInfo* meta = new TypeInfo(type_name, typeId);
        m_name2Types.emplace(type_name, meta);
        m_id2Types.emplace(typeId, meta);
        std::vector<TypeInfo*> baseClasses;
        RecvGetSuperClassMetas(type_name, baseClasses);
        m_id2BaseClassTypes[typeId] = baseClasses;
        return meta;
    }

    std::map<TypeID, std::vector<TypeInfo*>> TypeInfo::GetBaseClassInfos() { return m_id2BaseClassTypes; }

    void TypeInfo::Clear()
    {
        for (auto item : m_id2Types)
        {
            delete item.second;
        }
        m_name2Types.clear();
        m_id2Types.clear();
    }


    TypeInfo::TypeInfo(std::string type_name, TypeID typeId) : m_typeName(type_name)
    {
        m_isValid = false;
        m_fields.clear();
        m_methods.clear();
        m_typeId = typeId;
        auto fileds_iter = MetaRegisterUtil::m_fieldMap.equal_range(type_name);
        while (fileds_iter.first != fileds_iter.second)
        {
            FieldAccessor f_field(fileds_iter.first->second);
            m_fields.emplace_back(f_field);
            m_isValid = true;

            ++fileds_iter.first;
        }

        auto methods_iter = MetaRegisterUtil::m_methodMap.equal_range(type_name);
        while (methods_iter.first != methods_iter.second)
        {
            MethodAccessor f_method(methods_iter.first->second);
            m_methods.emplace_back(f_method);
            m_isValid = true;

            ++methods_iter.first;
        }
    }

    TypeInfo::TypeInfo() : m_typeName(k_unknown_type), m_isValid(false), m_typeId(0)
    {
        m_fields.clear();
        m_methods.clear();
    }


    bool TypeInfo::NewArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor)
    {
        auto iter = MetaRegisterUtil::m_arrayMap.find(array_type_name);

        if (iter != MetaRegisterUtil::m_arrayMap.end())
        {
            ArrayAccessor New_accessor(iter->second);
            accessor = New_accessor;
            return true;
        }

        return false;
    }

    void* TypeInfo::CreateFromNameAndJson(std::string type_name, const Json& json_context)
    {
        auto iter = MetaRegisterUtil::m_classMap.find(type_name);
        if (iter != MetaRegisterUtil::m_classMap.end())
        {
            return (iter->second->ConstructorWithJsonFunc(json_context));
        }
        return nullptr;
    }

    Json TypeInfo::WriteByName(std::string type_name, void* instance)
    {
        auto iter = MetaRegisterUtil::m_classMap.find(type_name);

        if (iter != MetaRegisterUtil::m_classMap.end())
        {
            return iter->second->WriteJsonByNameFunc(instance);
        }
        return Json();
    }

    std::string TypeInfo::GetTypeName() { return m_typeName; }

    TypeID TypeInfo::GetTypeID() const { return m_typeId; }

    int TypeInfo::GetFieldsList(FieldAccessor*& out_list)
    {
        int count = m_fields.size();
        out_list = new FieldAccessor[count];
        for (int i = 0; i < count; ++i)
        {
            out_list[i] = m_fields[i];
        }
        return count;
    }

    int TypeInfo::GetMethodsList(MethodAccessor*& out_list)
    {
        int count = m_methods.size();
        out_list = new MethodAccessor[count];
        for (int i = 0; i < count; ++i)
        {
            out_list[i] = m_methods[i];
        }
        return count;
    }


    std::vector<TypeID> TypeInfo::GetBaseClassIds(std::string type_name)
    {
        auto iter = MetaRegisterUtil::m_classMap.find(type_name);
        if (iter != MetaRegisterUtil::m_classMap.end())
        {
            return iter->second->GetBaseClassIds();
        }
        return std::vector<TypeID>();
    }


    FieldAccessor TypeInfo::GetFieldByName(const char* name)
    {
        const auto it = std::find_if(m_fields.begin(), m_fields.end(), [&](const auto& i)
        {
            return std::strcmp(i.GetFieldName(), name) == 0;
        });
        if (it != m_fields.end())
            return *it;
        return FieldAccessor(nullptr);
    }

    MethodAccessor TypeInfo::GetMethodByName(const char* name)
    {
        const auto it = std::find_if(m_methods.begin(), m_methods.end(), [&](const auto& i)
        {
            return std::strcmp(i.GetMethodName(), name) == 0;
        });
        if (it != m_methods.end())
            return *it;
        return MethodAccessor(nullptr);
    }

    bool TypeInfo::IsSubclassOf(TypeID typeId)
    {
        if (m_id2BaseClassTypes.count(typeId) == 0) return false;
        auto& suppers = m_id2BaseClassTypes.at(m_typeId);
        for (auto supper : suppers)
        {
            if (supper->m_typeId == typeId)
                return true;
        }
        return false;
    }

    bool TypeInfo::IsAssignableFrom(TypeID typeId)
    {
        return typeId == m_typeId || IsSubclassOf(typeId);
    }

    TypeInfo& TypeInfo::operator=(const TypeInfo& dest)
    {
        if (this == &dest)
        {
            return *this;
        }
        m_fields.clear();
        m_fields = dest.m_fields;


        m_methods.clear();
        m_methods = dest.m_methods;

        m_typeName = dest.m_typeName;
        m_isValid = dest.m_isValid;

        return *this;
    }
}
