#include "JsonSerializer.h"
#include "assert.h"
namespace LittleEngine
{
    
    std::map<TypeID,JsonSerializer::__FunPtrReadComponent> JsonSerializer::Id2ReadFunctionMap;
    std::map<TypeID,JsonSerializer::__FunPtrWriteComponent> JsonSerializer::Id2WriteFunctionMap;
    
    template<>
    Json JsonSerializer::Write(const char& instance)
    {
        return Json(instance);
    }
    template<>
    char& JsonSerializer::Read(const Json& json_context, char& instance)
    {
        assert(json_context.is_number());
        return instance = (char)json_context.number_value();
    }

    
    template<>
    Json JsonSerializer::Write(const int& instance)
    {
        return Json(instance);
    }
    template<>
    int& JsonSerializer::Read(const Json& json_context, int& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<int>(json_context.number_value());
    }
    
    template<>
    Json JsonSerializer::Write(const unsigned int& instance)
    {
        return Json(static_cast<int>(instance));
    }
    template<>
    unsigned int& JsonSerializer::Read(const Json& json_context, unsigned int& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<unsigned int>(json_context.number_value());
    }
    
    template<>
    Json JsonSerializer::Write(const int64_t& instance)
    {
        return Json(static_cast<double>(instance));
    }
    template<>
    int64_t& JsonSerializer::Read(const Json& json_context, int64_t& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<int64_t>(json_context.number_value());
    }
    template<>
    Json JsonSerializer::Write(const uint64_t& instance)
    {
        return Json(static_cast<double>(instance));
    }
    template<>
    uint64_t& JsonSerializer::Read(const Json& json_context, uint64_t& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<uint64_t>(json_context.number_value());
    }
    
    template<>
    Json JsonSerializer::Write(const float& instance)
    {
        return Json(instance);
    }
    template<>
    float& JsonSerializer::Read(const Json& json_context, float& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    template<>
    Json JsonSerializer::Write(const double& instance)
    {
        return Json(instance);
    }
    template<>
    double& JsonSerializer::Read(const Json& json_context, double& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    template<>
    Json JsonSerializer::Write(const bool& instance)
    {
        return Json(instance);
    }
    template<>
    bool& JsonSerializer::Read(const Json& json_context, bool& instance)
    {
        assert(json_context.is_bool());
        return instance = json_context.bool_value();
    }

    template<>
    Json JsonSerializer::Write(const std::string& instance)
    {
        return Json(instance);
    }
    template<>
    std::string& JsonSerializer::Read(const Json& json_context, std::string& instance)
    {
        assert(json_context.is_string());
        return instance = json_context.string_value();
    }


} // namespace LittleEngine
