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
            {"guid", Json( instance.GetGUID())}\
        };\
    }\
    template<>\
    restypename##ResPtr& JsonSerializer::Read(const Json& json_context, restypename##ResPtr& instance)\
    {\
        if(json_context["guid"].is_null()){instance.Reset("",nullptr);return instance;} \
        auto guid = json_context["guid"].string_value();\
        auto ptr = ResourcesUtils::Load##restypename##(guid);\
        instance.Reset(guid,ptr);\
        return instance;\
    }
    
    ShaderResPtr ShaderResPtr::NullPtr = ShaderResPtr();

    template <>
    Json JsonSerializer::Write(const ShaderResPtr& instance)
    {
        auto resType = instance.GetResType();
        return Json::object{{"guid", Json(instance.GetGUID())}};
    }

    template <>
    ShaderResPtr& JsonSerializer::Read(const Json& json_context, ShaderResPtr& instance)
    {
        if (json_context["guid"].is_null()) { instance.Reset("", nullptr); }
        auto guid = json_context["guid"].string_value();
        auto ptr = ResourcesUtils::LoadShader(guid);
        instance.Reset(guid, ptr);
        return instance;
    }
   
    //DEFINE_RES_PTR_JSON_SERIALIZER(Shader) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Texture)
    DEFINE_RES_PTR_JSON_SERIALIZER(Model) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Animation) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Sound) 
    DEFINE_RES_PTR_JSON_SERIALIZER(Material)
    
}