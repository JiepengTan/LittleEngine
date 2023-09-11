#pragma once
#include "MetaFunctionPointer.h"
#include "MetaMacro.h"
// copy from https://github.com/BoomingTech/Piccolo
namespace LittleEngine::Reflection
{
    const static char* k_unknown_type = "UnknownType";
    const static char* k_unknown      = "Unknown";
    
#define META_REGISTER_FIELD_TO_MAP(name, value) LittleEngine::Reflection::MetaRegisterUtil::RegisterToFieldMap(name, value);
#define META_REGISTER_Method_TO_MAP(name, value) LittleEngine::Reflection::MetaRegisterUtil::RegisterToMethodMap(name, value);
#define META_REGISTER_BASE_CLASS_TO_MAP(name, value, typeId) LittleEngine::Reflection::MetaRegisterUtil::RegisterToClassMap(name, value, typeId);
#define META_REGISTER_ARRAY_TO_MAP(name, value) LittleEngine::Reflection::MetaRegisterUtil::RegisterToArrayMap(name, value);
#define META_REGISTER_META_TYPE_INFO(name, typeId) LittleEngine::Reflection::MetaRegisterUtil::RegisterType(name, typeId);;
#define META_UNREGISTER_ALL LittleEngine::Reflection::MetaRegisterUtil::UnRegisterAll();
    
    class TypeInfo;
    class MetaRegisterUtil
    {
        friend class TypeInfo;
        
        static std::map<TypeID, std::string>                    m_id2NameMap;
        static std::map<std::string,TypeID>                     m_name2IdMap;
        static std::map<std::string, ClassFunctionTuple*>       m_name2ClassMap;
        static std::multimap<std::string, FieldFunctionTuple*>  m_name2FieldMap;
        static std::multimap<std::string, MethodFunctionTuple*> m_name2MethodMap;
        static std::map<std::string, ArrayFunctionTuple*>       m_name2ArrayMap;
        
        static std::map<TypeID, ClassFunctionTuple*>            m_id2ClassMap;
        static std::multimap<TypeID, FieldFunctionTuple*>       m_id2FieldMap;
        static std::multimap<TypeID, MethodFunctionTuple*>      m_id2MethodMap;
        static std::map<TypeID, ArrayFunctionTuple*>            m_id2ArrayMap;
        
        static void RegisterAllCodeGen();
    public:
        static void RegisterToClassMap(const char* type_name, ClassFunctionTuple* value, TypeID typeId);
        static void RegisterToFieldMap(const char* name, FieldFunctionTuple* value);

        static void RegisterToMethodMap(const char* name, MethodFunctionTuple* value);
        static void RegisterToArrayMap(const char* name, ArrayFunctionTuple* value);
        static TypeInfo* RegisterType(std::string type_name,TypeID typeId);
        static std::string GetTypeName(TypeID typeId);
    public:
        static void RegisterAll();
        static void UnRegisterAll();
    };

} // namespace LittleEngine
