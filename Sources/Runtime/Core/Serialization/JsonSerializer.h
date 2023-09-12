#pragma once
#include "Core/Serialization/Json.h"
#include "Core/Reflection/Reflection.h"

#include "cassert"

#include "ISerializer.h"

namespace LittleEngine
{
    template<typename...>
    inline constexpr bool always_false = false;
    class Component;
    class JsonSerializer 
    {
    public:
        typedef Json (* __FunPtrWriteComponent)(void* data);
        typedef void (* __FunPtrReadComponent)(const Json& jsonContext,void* data);
        static std::map<TypeID,__FunPtrReadComponent> Id2ReadFunctionMap;
        static std::map<TypeID,__FunPtrWriteComponent> Id2WriteFunctionMap;
        
    public:
        
        template<typename T>
        static void ReadEnum(const Json& jsonContext, T& instance)
        {
            assert(jsonContext.is_number());
            instance = (T)(int)(jsonContext.number_value());
        }
        template<typename T>
        static Json WriteEnum(T& instance)
        {
            return Json((int)instance);
        }
        
        template<typename T>
        static Json WriteRes(T* instance)
        {
            static_assert(always_false<T>, "Serializer::Write<T> has not been implemented yet!"); 
            return Json();
        }
        template<typename T>
        static void ReadRes(const Json& jsonContext, T*& instance)
        {
            static_assert(always_false<T>, "Serializer::Read<T> has not been implemented yet!");  
        }
        
        template<typename T>
        static Json WritePointer(T* instance)
        {
            return Json::object {{"$typeName", Json {"*"}}, {"$context", JsonSerializer::Write(*instance)}};
        }

        template<typename T>
        static T*& ReadPointer(const Json& jsonContext, T*& instance)
        {
            assert(instance == nullptr);
            std::string type_name = jsonContext["$typeName"].string_value();
            assert(!type_name.empty());
            if ('*' == type_name[0])
            {
                instance = new T;
                Read(jsonContext["$context"], *instance);
            }
            else
            {
                auto inst = Reflection::TypeInfo::CreateFromNameAndJson(type_name, jsonContext["$context"]);
                instance = static_cast<T*>(inst);
            }
            return instance;
        }

        template<typename T>
        static Json Write(const Reflection::MetaPtr<T>& instance)
        {
            T*          instance_ptr = static_cast<T*>(instance.operator->());
            std::string type_name    = instance.GetTypeName();
            return Json::object {{"$typeName", Json(type_name)},
                                  {"$context", Reflection::TypeInfo::WriteByName(type_name, instance_ptr)}};
        }

        template<typename T>
        static T*& Read(const Json& jsonContext, Reflection::MetaPtr<T>& instance)
        {
            std::string type_name = jsonContext["$typeName"].string_value();
            instance.SetTypeName(type_name);
            return ReadPointer(jsonContext, instance.GetPtrReference());
        }
        template<typename T>
        static Json Write(const std::vector<T>& instance)
        {
            Json::array jsonAry;
            for (auto& item : instance){
                jsonAry.emplace_back(JsonSerializer::Write(item));
            }
            return Json(jsonAry);
        }
        template<typename T>
        static std::vector<T>& Read(const Json& jsonContext, std::vector<T>& vec)
        {
            assert(jsonContext.is_array());
            Json::array jsonAry = jsonContext.array_items();
            vec.resize(jsonAry.size());
            for (size_t index=0; index < jsonAry.size();++index){
                JsonSerializer::Read(jsonAry[index], vec[index]);
            }
            return vec;
        }
        template<typename K,typename V>
        static Json Write(const std::map<K,V>& instance)
        {
            Json::array jsonAry;
            for (auto& item : instance){
                jsonAry.emplace_back(JsonSerializer::Write(item.first));
                jsonAry.emplace_back(JsonSerializer::Write(item.second));
            }
            return Json(jsonAry);
        }
        template<typename K,typename V>
        static std::map<K,V>& Read(const Json& jsonContext, std::map<K,V>& instance)
        {
            assert(jsonContext.is_array());
            Json::array jsonAry = jsonContext.array_items();
            instance.resize(jsonAry.size()/2);
            for (size_t index=0; index < jsonAry.size()/2;++index){
                K key;
                V val;
                JsonSerializer::Read(jsonAry[index], key);
                JsonSerializer::Read(jsonAry[index], val);
                instance[key] = val;
            }
            return instance;
        }
        
        
        template<typename T>
        static Json Write(const T& instance)
        {
            if constexpr (std::is_pointer<T>::value)
            {
                return WritePointer((T)instance);
            }
            else
            {
                static_assert(always_false<T>, "Serializer::Write<T> has not been implemented yet!");  
                return Json();
            }
        }
        
        template<typename T>
        static T& Read(const Json& jsonContext, T& instance)
        {
            if constexpr (std::is_pointer<T>::value)
            {
                return ReadPointer(jsonContext, instance);
            }
            else
            {
                static_assert(always_false<T>, "Serializer::Read<T> has not been implemented yet!");
                return instance;
            }
        }

    };

    
    
    // implementation of base types
    template<>
    Json JsonSerializer::Write(const char& instance);
    template<>
    char& JsonSerializer::Read(const Json& jsonContext, char& instance);

    
    template<>
    Json JsonSerializer::Write(const int& instance);
    template<>
    int& JsonSerializer::Read(const Json& jsonContext, int& instance);
    
    template<>
    Json JsonSerializer::Write(const unsigned int& instance);
    template<>
    unsigned int& JsonSerializer::Read(const Json& jsonContext, unsigned int& instance);
    
    template<>
    Json JsonSerializer::Write(const int64_t& instance);
    template<>
    int64_t& JsonSerializer::Read(const Json& jsonContext, int64_t& instance);
    
    template<>
    Json JsonSerializer::Write(const uint64_t& instance);
    template<>
    uint64_t& JsonSerializer::Read(const Json& jsonContext, uint64_t& instance);

    
    template<>
    Json JsonSerializer::Write(const float& instance);
    template<>
    float& JsonSerializer::Read(const Json& jsonContext, float& instance);

    template<>
    Json JsonSerializer::Write(const double& instance);
    template<>
    double& JsonSerializer::Read(const Json& jsonContext, double& instance);

    template<>
    Json JsonSerializer::Write(const bool& instance);
    template<>
    bool& JsonSerializer::Read(const Json& jsonContext, bool& instance);

    template<>
    Json JsonSerializer::Write(const std::string& instance);
    template<>
    std::string& JsonSerializer::Read(const Json& jsonContext, std::string& instance);

} // namespace LittleEngine
