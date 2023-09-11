#include "assert.h"

#include "Core/Reflection/Reflection.h"
#include "MetaRegisterUtil.h"

#include "_Generated/Reflection/AllReflection.h"
#include "_Generated/Serializer/AllSerializer.ipp"
namespace LittleEngine::Reflection
{
    std::map<TypeID, std::string>                    MetaRegisterUtil::m_id2ClassNameMap;
    std::map<std::string, ClassFunctionTuple*>       MetaRegisterUtil::m_classMap;
    std::multimap<std::string, FieldFunctionTuple*>  MetaRegisterUtil::m_fieldMap;
    std::multimap<std::string, MethodFunctionTuple*> MetaRegisterUtil::m_methodMap;
    std::map<std::string, ArrayFunctionTuple*>       MetaRegisterUtil::m_arrayMap;

    
    TypeInfo* MetaRegisterUtil::RegisterType(
        std::string type_name, TypeID typeId)
    {
        return TypeInfo::RegisterType(type_name, typeId);
    }
        
    void MetaRegisterUtil::RegisterToFieldMap(const char* name, FieldFunctionTuple* value)
    {
        m_fieldMap.insert(std::make_pair(name, value));
    }
    void MetaRegisterUtil::RegisterToMethodMap(const char* name, MethodFunctionTuple* value)
    {
        m_methodMap.insert(std::make_pair(name, value));
    }
    void MetaRegisterUtil::RegisterToArrayMap(const char* name, ArrayFunctionTuple* value)
    {
        if (m_arrayMap.find(name) == m_arrayMap.end())
        {
            m_arrayMap.insert(std::make_pair(name, value));
        }
        else
        {
            delete value;
        }
    }

    std::string MetaRegisterUtil::GetTypeName(TypeID typeId)
    {
        if (m_id2ClassNameMap.find(typeId) != m_id2ClassNameMap.end())
        {
            return m_id2ClassNameMap.at(typeId);
        }
        return k_unknown_type;
    }

    void MetaRegisterUtil::RegisterToClassMap(const char* name, ClassFunctionTuple* value, TypeID typeId)
    {
        if (m_id2ClassNameMap.find(typeId) == m_id2ClassNameMap.end())
        {
            m_id2ClassNameMap.insert(std::make_pair(typeId, std::string(name)));
        }
            
        if (m_classMap.find(name) == m_classMap.end())
        {
            m_id2ClassNameMap.insert(std::make_pair(typeId, std::string(name)));
            m_classMap.insert(std::make_pair(name, value));
        }
        else
        {
            delete value;
        }
    }
        

    void MetaRegisterUtil::UnRegisterAll()
    {
        for (const auto& itr : m_fieldMap)
        {
            delete itr.second;
        }
        m_fieldMap.clear();
        for (const auto& itr : m_classMap)
        {
            delete itr.second;
        }
        m_classMap.clear();
        for (const auto& itr : m_arrayMap)
        {
            delete itr.second;
        }
        m_arrayMap.clear();
    }
} // namespace LittleEngine
