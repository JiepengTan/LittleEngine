#pragma once
// copy from https://github.com/BoomingTech/Piccolo
#include "Core/Serialization/Json.h"

#include "functional"
#include "string"
#include "unordered_map"
#include "unordered_Set"
#include "vector"

#include "Core/Base/Macro.h"

namespace LittleEngine
{

#if defined(__REFLECTION_PARSER__)
    #define META(...) __attribute__((annotate(#__VA_ARGS__)))
    #define CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
    #define STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name
#else
    #define META(...)
    #define CLASS(class_name, ...) class class_name
    #define STRUCT(struct_name, ...) struct struct_name
#endif // __REFLECTION_PARSER__

    

        
#define REFLECTION_BASIC_TYPE(class_name) \
    namespace MetaCodeGen \
    { \
        class TypeOperator_##class_name; \
    };\

#define REFLECTION_BASIC_BODY(class_name) \
    friend class MetaCodeGen::TypeOperator_##class_name; \
    friend class JsonSerializer;\
    public :\
    static ::LittleEngine::TypeID MetaTypeId;\
    static LittleEngine::Reflection::TypeInfo* GetStaticType(){ return LittleEngine::Reflection::TypeInfo::GetType(GetStaticTypeID()); }\
    static bool IsStaticSubclassOf(::LittleEngine::TypeID typeId){ return GetStaticType()->IsSubclassOf(typeId); }\
    static bool IsStaticAssignableFrom(::LittleEngine::TypeID typeId){ return GetStaticType()->IsAssignableFrom(typeId); }\
    static ::LittleEngine::TypeID GetStaticTypeID(){ return class_name##::MetaTypeId;}\
    static std::string GetStaticTypeName(){ return LittleEngine::Reflection::MetaRegisterUtil::GetTypeName(class_name##::MetaTypeId);}

#define REFLECTION_TYPE(class_name)\
    REFLECTION_BASIC_TYPE(class_name)

#define REFLECTION_BODY(class_name) \
    REFLECTION_BASIC_BODY(class_name)\
    virtual std::string ToString();\
    virtual Json ToJson();\

    
#define REFLECTION_STRUCT_TYPE(class_name) \
    REFLECTION_BASIC_TYPE(class_name)

//!!! pure data type should not define a virtual function
// that would make compiler create a virtual function table pointer
// which would cause memory alignment bug !
#define REFLECTION_STRUCT_BODY(class_name) \
    REFLECTION_BASIC_BODY(class_name)\
    std::string ToString();\
    Json ToJson();\

    
#define REFLECTION_COMPONENT_TYPE(class_name) \
    REFLECTION_BASIC_TYPE(class_name)

#define REFLECTION_COMPONENT_BODY(class_name) \
    REFLECTION_BODY(class_name)\
    \
    ##class_name(){\
        _LE_InternalMetaTypeID = class_name##::MetaTypeId;\
        OnConstruction();\
    }\
    virtual LittleEngine::Reflection::TypeInfo* GetType(){ return LittleEngine::Reflection::TypeInfo::GetType(_LE_InternalMetaTypeID); }\
    virtual bool IsSubclassOf(::LittleEngine::TypeID typeId){ return GetType()->IsSubclassOf(typeId); }\
    virtual bool IsAssignableFrom(::LittleEngine::TypeID typeId){ return GetType()->IsAssignableFrom(typeId); }\
    virtual ::LittleEngine::TypeID GetTypeID(){ return _LE_InternalMetaTypeID;}\
    virtual std::string GetTypeName(){ return LittleEngine::Reflection::MetaRegisterUtil::GetTypeName(_LE_InternalMetaTypeID);}


} // namespace LittleEngine
