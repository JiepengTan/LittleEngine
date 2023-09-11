#include "MetaRegisterEditorUtil.h"

#include "TypeMeta.h"

namespace LittleEngine::Reflection
{
    
    std::map<TypeID, std::string>                    MetaRegisterEditorUtil::m_id2ClassNameMap;
    std::map<std::string, ClassFunctionTuple*>       MetaRegisterEditorUtil::m_classMap;
    std::multimap<std::string, FieldFunctionTuple*>  MetaRegisterEditorUtil::m_fieldMap;
    std::multimap<std::string, MethodFunctionTuple*> MetaRegisterEditorUtil::m_methodMap;
    std::map<std::string, ArrayFunctionTuple*>       MetaRegisterEditorUtil::m_arrayMap;

    
    TypeMeta* MetaRegisterEditorUtil::RegisterType(
        std::string type_name, TypeID typeId)
    {
        return TypeMeta::RegisterType(type_name, typeId);
    }
        
    void MetaRegisterEditorUtil::RegisterToFieldMap(const char* name, FieldFunctionTuple* value)
    {
        m_fieldMap.insert(std::make_pair(name, value));
    }
    void MetaRegisterEditorUtil::RegisterToMethodMap(const char* name, MethodFunctionTuple* value)
    {
        m_methodMap.insert(std::make_pair(name, value));
    }
    void MetaRegisterEditorUtil::RegisterToArrayMap(const char* name, ArrayFunctionTuple* value)
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

    std::string MetaRegisterEditorUtil::GetTypeName(TypeID typeId)
    {
        if (m_id2ClassNameMap.find(typeId) != m_id2ClassNameMap.end())
        {
            return m_id2ClassNameMap.at(typeId);
        }
        return k_unknown_type;
    }

    void MetaRegisterEditorUtil::RegisterToClassMap(const char* name, ClassFunctionTuple* value, TypeID typeId)
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
        

    void MetaRegisterEditorUtil::UnRegisterAll()
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
}
