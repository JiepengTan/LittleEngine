#include "ResPtr.h"

namespace LittleEngine
{
    
#define DEFINE_RES_PTR_JSON_SERIALIZER(restypename)    \
    restypename##ResPtr  restypename##ResPtr::NullPtr = restypename##ResPtr();\
    template<>\
    Json JsonSerializer::Write(const restypename##ResPtr& instance)\
    {\
        auto resType    = instance.GetResType();\
        return Json::object {\
            {"resType", Json((int)resType)},\
            {"guid", Json( instance.GetGUID())}\
        };\
    }\
    template<>\
    restypename##ResPtr& JsonSerializer::Read(const Json& json_context, restypename##ResPtr& instance)\
    {\
        auto resType = (EResType)json_context["resType"].int_value();\
        auto guid = json_context["guid"].string_value();\
        auto ptr = ResourcesUtils::Load##restypename##(guid);\
        instance.Reset(resType,guid,ptr);\
        return instance;\
    }


    DEFINE_RES_PTR_JSON_SERIALIZER(Shader) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Texture)
    DEFINE_RES_PTR_JSON_SERIALIZER(Model) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Animation) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Sound) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Material)
    
}