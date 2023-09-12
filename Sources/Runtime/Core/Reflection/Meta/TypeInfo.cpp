// copy from https://github.com/BoomingTech/Piccolo
#include "Core/Base/Macro.h"
#include "cstring"
#include "map"
#include "MetaAccessor.h"
#include "MetaRegisterUtil.h"
#include "TypeInfo.h"


namespace LittleEngine::Reflection
{
    TVector<TypeInfo*> TypeInfo::m_allTypes;
    std::map<TypeID, TypeInfo*> TypeInfo::m_id2Types;
    std::map<std::string, TypeInfo*> TypeInfo::m_name2Types;
    std::map<TypeID, std::vector<TypeInfo*>> TypeInfo::m_id2BaseClassTypes;

    
    void* TypeInfo::CreateInstance()
    {
        return MetaRegisterUtil::m_id2ClassMap.at(GetTypeID())->Constructor();
    }
    void* TypeInfo::CreateInstance(TypeID typeId)
    {
        auto type = GetType(typeId);
        if(type == nullptr)
            return nullptr;
        return type->CreateInstance();
    }
    void* TypeInfo::CreateInstance(std::string typeName)
    {
        auto type = GetType(typeName);
        if(type == nullptr)
            return nullptr;
        return type->CreateInstance();
    }

    bool TypeInfo::HasMeta(std::string metaKey)
    {
        return m_metaData.count(metaKey) != 0;
    }

    std::string TypeInfo::GetMeta(std::string metaKey)
    {
        if(m_metaData.count(metaKey) == 0) return "";
        return m_metaData.at(metaKey);
    }

    TypeInfo* TypeInfo::GetType(std::string typeName)
    {
        if (m_name2Types.count(typeName) == 0) return nullptr;
        return m_name2Types.at(typeName);
    }

    TypeInfo* TypeInfo::GetType(TypeID typeId)
    {
        if (m_id2Types.count(typeId) == 0) return nullptr;
        return m_id2Types.at(typeId);
    }
    bool TypeInfo::IsAbstract(TypeID typeId)
    {
        if (m_id2Types.count(typeId) == 0) return false;
        return GetType(typeId)->IsAbstract();
    }
    bool TypeInfo::IsAbstract(std::string typeName)
    {
        if (m_name2Types.count(typeName) == 0) return false;
        return GetType(typeName)->IsAbstract();
    }
    bool TypeInfo::HasType(TypeID typeId)
    {
        return m_id2Types.count(typeId) != 0;
    }

    bool TypeInfo::HasType(std::string typeName)
    {      
        return m_name2Types.count(typeName) != 0;
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

    void TypeInfo::RecvGetSuperClassMetas(std::string typeName, std::vector<TypeInfo*>& supperClasses)
    {
        auto ids = GetBaseClassIds(typeName);
        for (auto id : ids)
        {
            if (m_id2Types.count(id) == 0)
            {
                auto className = MetaRegisterUtil::m_id2NameMap.at(id);
                auto base = RegisterType(className, id);
                supperClasses.push_back(base);
            }
            else
            {
                supperClasses.push_back(m_id2Types.at(id));
            }
            RecvGetSuperClassMetas(MetaRegisterUtil::m_id2NameMap.at(id), supperClasses);
        }
    }

    TypeInfo* TypeInfo::RegisterType(std::string typeName, TypeID typeId)
    {
        if (m_id2Types.count(typeId) != 0) return m_id2Types.at(typeId);
        TypeInfo* meta = new TypeInfo(typeName, typeId);
        meta->m_metaData = MetaRegisterUtil::m_name2ClassMap.at(typeName)->GetMetaData();
        m_name2Types.emplace(typeName, meta);
        m_id2Types.emplace(typeId, meta);
        std::vector<TypeInfo*> baseClasses;
        RecvGetSuperClassMetas(typeName, baseClasses);
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


    TypeInfo::TypeInfo(std::string typeName, TypeID typeId) : m_typeName(typeName)
    {
        m_isValid = false;
        m_fields.clear();
        m_methods.clear();
        m_typeId = typeId;
        auto fileds_iter = MetaRegisterUtil::m_name2FieldMap.equal_range(typeName);
        while (fileds_iter.first != fileds_iter.second)
        {
            FieldAccessor f_field(fileds_iter.first->second);
            m_fields.emplace_back(f_field);
            m_isValid = true;

            ++fileds_iter.first;
        }

        auto methods_iter = MetaRegisterUtil::m_name2MethodMap.equal_range(typeName);
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


    bool TypeInfo::NewArrayAccessorFromName(std::string arrayTypeName, ArrayAccessor& accessor)
    {
        auto iter = MetaRegisterUtil::m_name2ArrayMap.find(arrayTypeName);

        if (iter != MetaRegisterUtil::m_name2ArrayMap.end())
        {
            ArrayAccessor New_accessor(iter->second);
            accessor = New_accessor;
            return true;
        }

        return false;
    }

    void* TypeInfo::CreateFromNameAndJson(std::string typeName, const Json& jsonContext)
    {
        auto iter = MetaRegisterUtil::m_name2ClassMap.find(typeName);
        if (iter != MetaRegisterUtil::m_name2ClassMap.end())
        {
            return (iter->second->ConstructorWithJson(jsonContext));
        }
        return nullptr;
    }

    Json TypeInfo::WriteByName(std::string typeName, void* instance)
    {
        auto iter = MetaRegisterUtil::m_name2ClassMap.find(typeName);

        if (iter != MetaRegisterUtil::m_name2ClassMap.end())
        {
            return iter->second->WriteJsonByName(instance);
        }
        return Json();
    }

    std::string TypeInfo::GetTypeName() { return m_typeName; }

    TypeID TypeInfo::GetTypeID() const { return m_typeId; }

    int TypeInfo::GetFieldsList(FieldAccessor*& outList)
    {
        int count = m_fields.size();
        outList = new FieldAccessor[count];
        for (int i = 0; i < count; ++i)
        {
            outList[i] = m_fields[i];
        }
        return count;
    }

    int TypeInfo::GetMethodsList(MethodAccessor*& outList)
    {
        int count = m_methods.size();
        outList = new MethodAccessor[count];
        for (int i = 0; i < count; ++i)
        {
            outList[i] = m_methods[i];
        }
        return count;
    }


    std::vector<TypeID> TypeInfo::GetBaseClassIds(std::string typeName)
    {
        auto iter = MetaRegisterUtil::m_name2ClassMap.find(typeName);
        if (iter != MetaRegisterUtil::m_name2ClassMap.end())
        {
            return iter->second->GetBaseClassIds();
        }
        return std::vector<TypeID>();
    }


    FieldAccessor* TypeInfo::GetFieldByName(std::string name)
    {
        for (auto& element : m_fields)
        {
            if(element.GetFieldName() == name) return &element;
        }
        return nullptr;
    }

    MethodAccessor* TypeInfo::GetMethodByName(std::string name)
    {
        for (auto& element : m_methods)
        {
            if(element.GetMethodName() == name) return &element;
        }
        return nullptr;
    }

    bool TypeInfo::IsSubclassOf(TypeID typeId)
    {
        if (!HasType(typeId))
            return false;
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

    bool TypeInfo::IsAbstract()
    {
        return MetaRegisterUtil::m_id2ClassMap.at(GetTypeID())->IsAbstract();
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
