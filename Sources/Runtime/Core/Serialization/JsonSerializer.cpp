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
    char& JsonSerializer::Read(const Json& jsonContext, char& instance)
    {
        assert(jsonContext.is_number());
        return instance = (char)jsonContext.number_value();
    }

    
    template<>
    Json JsonSerializer::Write(const int& instance)
    {
        return Json(instance);
    }
    template<>
    int& JsonSerializer::Read(const Json& jsonContext, int& instance)
    {
        assert(jsonContext.is_number());
        return instance = static_cast<int>(jsonContext.number_value());
    }
    
    template<>
    Json JsonSerializer::Write(const unsigned int& instance)
    {
        return Json(static_cast<int>(instance));
    }
    template<>
    unsigned int& JsonSerializer::Read(const Json& jsonContext, unsigned int& instance)
    {
        assert(jsonContext.is_number());
        return instance = static_cast<unsigned int>(jsonContext.number_value());
    }
    
    template<>
    Json JsonSerializer::Write(const int64_t& instance)
    {
        return Json(static_cast<double>(instance));
    }
    template<>
    int64_t& JsonSerializer::Read(const Json& jsonContext, int64_t& instance)
    {
        assert(jsonContext.is_number());
        return instance = static_cast<int64_t>(jsonContext.number_value());
    }
    template<>
    Json JsonSerializer::Write(const uint64_t& instance)
    {
        return Json(static_cast<double>(instance));
    }
    template<>
    uint64_t& JsonSerializer::Read(const Json& jsonContext, uint64_t& instance)
    {
        assert(jsonContext.is_number());
        return instance = static_cast<uint64_t>(jsonContext.number_value());
    }
    
    template<>
    Json JsonSerializer::Write(const float& instance)
    {
        return Json(instance);
    }
    template<>
    float& JsonSerializer::Read(const Json& jsonContext, float& instance)
    {
        assert(jsonContext.is_number());
        return instance = static_cast<float>(jsonContext.number_value());
    }

    template<>
    Json JsonSerializer::Write(const double& instance)
    {
        return Json(instance);
    }
    template<>
    double& JsonSerializer::Read(const Json& jsonContext, double& instance)
    {
        assert(jsonContext.is_number());
        return instance = static_cast<float>(jsonContext.number_value());
    }

    template<>
    Json JsonSerializer::Write(const bool& instance)
    {
        return Json(instance);
    }
    template<>
    bool& JsonSerializer::Read(const Json& jsonContext, bool& instance)
    {
        assert(jsonContext.is_bool());
        return instance = jsonContext.bool_value();
    }

    template<>
    Json JsonSerializer::Write(const std::string& instance)
    {
        return Json(instance);
    }
    template<>
    std::string& JsonSerializer::Read(const Json& jsonContext, std::string& instance)
    {
        assert(jsonContext.is_string());
        return instance = jsonContext.string_value();
    }


} // namespace LittleEngine
