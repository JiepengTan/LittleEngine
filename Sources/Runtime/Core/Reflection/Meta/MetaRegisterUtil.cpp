#include "assert.h"

#include "Core/Reflection/Reflection.h"
#include "TypeInfo.h"
#include "MetaRegisterUtil.h"

#include "_Generated/Reflection/AllReflection.h"
#include "_Generated/Serializer/AllSerializer.ipp"
namespace LittleEngine::Reflection
{
    std::map<std::string,TypeID>                     MetaRegisterUtil::m_name2IdMap;
    std::map<std::string, ClassFunctionTuple*>       MetaRegisterUtil::m_name2ClassMap;
    std::multimap<std::string, FieldFunctionTuple*>  MetaRegisterUtil::m_name2FieldMap;
    std::multimap<std::string, MethodFunctionTuple*> MetaRegisterUtil::m_name2MethodMap;
    std::map<std::string, ArrayFunctionTuple*>       MetaRegisterUtil::m_name2ArrayMap;
    
    std::map<TypeID, std::string>                    MetaRegisterUtil::m_id2NameMap;
    std::map<TypeID, ClassFunctionTuple*>            MetaRegisterUtil::m_id2ClassMap;
    std::multimap<TypeID, FieldFunctionTuple*>       MetaRegisterUtil::m_id2FieldMap;
    std::multimap<TypeID, MethodFunctionTuple*>      MetaRegisterUtil::m_id2MethodMap;
    std::map<TypeID, ArrayFunctionTuple*>            MetaRegisterUtil::m_id2ArrayMap;


    
#define  __ClearContainer(mapContainer)\
    for (auto ident : ##mapContainer)\
        delete ident.second;\
    ##mapContainer.clear();
    
#define  __CopyNameMap2IdMap( mapName)\
    for (auto ident : m_name2##mapName)\
        m_id2##mapName.emplace(m_name2IdMap[ident.first],ident.second);
    
    void MetaRegisterUtil::RegisterAll()
    {
        RegisterAllCodeGen();
        for (auto ident : m_name2IdMap )
            m_id2NameMap.emplace(ident.second,ident.first);
        // Build inheritance relationship 
        for (auto ident : m_name2IdMap)
            TypeInfo::RegisterType(ident.first, ident.second);
         

        __CopyNameMap2IdMap(MethodMap)
        __CopyNameMap2IdMap(FieldMap)
        __CopyNameMap2IdMap(ClassMap)
        __CopyNameMap2IdMap(ArrayMap);
    }
    
    void MetaRegisterUtil::UnRegisterAll()
    {
        m_id2NameMap.clear();
        m_name2IdMap.clear();
        
        __ClearContainer(m_name2MethodMap)
        __ClearContainer(m_name2FieldMap)
        __ClearContainer(m_name2ClassMap)
        __ClearContainer(m_name2ArrayMap)

        __ClearContainer(m_id2ClassMap)
        __ClearContainer(m_id2FieldMap)
        __ClearContainer(m_id2MethodMap)
        __ClearContainer(m_id2ArrayMap)
    }
    
    
    void MetaRegisterUtil::RegisterToFieldMap(const char* name, FieldFunctionTuple* value)
    {
        m_name2FieldMap.insert(std::make_pair(name, value));
    }
    void MetaRegisterUtil::RegisterToMethodMap(const char* name, MethodFunctionTuple* value)
    {
        m_name2MethodMap.insert(std::make_pair(name, value));
    }
    void MetaRegisterUtil::RegisterToArrayMap(const char* name, ArrayFunctionTuple* value)
    {
        if (m_name2ArrayMap.find(name) == m_name2ArrayMap.end())
        {
            m_name2ArrayMap.insert(std::make_pair(name, value));
        }
        else
        {
            delete value;
        }
    }


    void MetaRegisterUtil::RegisterToClassMap(const char* type_name, ClassFunctionTuple* value, TypeID typeId)
    {
        if (m_name2IdMap.find(type_name) == m_name2IdMap.end())
        {
            m_name2IdMap.insert(std::make_pair( std::string(type_name),typeId));
            m_name2ClassMap.insert(std::make_pair(type_name, value));
        }
    }
        
    std::string MetaRegisterUtil::GetTypeName(TypeID typeId)
    {
        if (m_id2NameMap.find(typeId) != m_id2NameMap.end())
        {
            return m_id2NameMap.at(typeId);
        }
        return k_unknown_type;
    }
    
} 
