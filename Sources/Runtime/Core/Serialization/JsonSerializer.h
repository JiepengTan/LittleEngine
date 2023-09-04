#pragma once
#include "Core/Serialization/Json.h"
#include "Core/Reflection/Reflection.h"

#include <cassert>

namespace LittleEngine
{
    template<typename...>
    inline constexpr bool always_false = false;

    class JsonSerializer
    {
    public:
        template<typename T>
        static Json WritePointer(T* instance)
        {
            return Json::object {{"$typeName", Json {"*"}}, {"$context", JsonSerializer::Write(*instance)}};
        }

        template<typename T>
        static T*& ReadPointer(const Json& json_context, T*& instance)
        {
            assert(instance == nullptr);
            std::string type_name = json_context["$typeName"].string_value();
            assert(!type_name.empty());
            if ('*' == type_name[0])
            {
                instance = new T;
                Read(json_context["$context"], *instance);
            }
            else
            {
                instance = static_cast<T*>(
                    Reflection::TypeMeta::NewFromNameAndJson(type_name, json_context["$context"]).m_instance);
            }
            return instance;
        }

        template<typename T>
        static Json Write(const Reflection::ReflectionPtr<T>& instance)
        {
            T*          instance_ptr = static_cast<T*>(instance.operator->());
            std::string type_name    = instance.GetTypeName();
            return Json::object {{"$typeName", Json(type_name)},
                                  {"$context", Reflection::TypeMeta::WriteByName(type_name, instance_ptr)}};
        }

        template<typename T>
        static T*& Read(const Json& json_context, Reflection::ReflectionPtr<T>& instance)
        {
            std::string type_name = json_context["$typeName"].string_value();
            instance.SetTypeName(type_name);
            return ReadPointer(json_context, instance.GetPtrReference());
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
                //static_assert(always_false<T>, "Serializer::Write<T> has not been implemented yet!"); // TODO tanjp open it after implement code gen 
                return Json();
            }
        }

        template<typename T>
        static T& Read(const Json& json_context, T& instance)
        {
            if constexpr (std::is_pointer<T>::value)
            {
                return ReadPointer(json_context, instance);
            }
            else
            {
                //static_assert(always_false<T>, "Serializer::Read<T> has not been implemented yet!");// TODO tanjp open it after implement code gen
                return instance;
            }
        }
    };

    // implementation of base types
    template<>
    Json JsonSerializer::Write(const char& instance);
    template<>
    char& JsonSerializer::Read(const Json& json_context, char& instance);

    template<>
    Json JsonSerializer::Write(const int& instance);
    template<>
    int& JsonSerializer::Read(const Json& json_context, int& instance);

    template<>
    Json JsonSerializer::Write(const unsigned int& instance);
    template<>
    unsigned int& JsonSerializer::Read(const Json& json_context, unsigned int& instance);

    template<>
    Json JsonSerializer::Write(const float& instance);
    template<>
    float& JsonSerializer::Read(const Json& json_context, float& instance);

    template<>
    Json JsonSerializer::Write(const double& instance);
    template<>
    double& JsonSerializer::Read(const Json& json_context, double& instance);

    template<>
    Json JsonSerializer::Write(const bool& instance);
    template<>
    bool& JsonSerializer::Read(const Json& json_context, bool& instance);

    template<>
    Json JsonSerializer::Write(const std::string& instance);
    template<>
    std::string& JsonSerializer::Read(const Json& json_context, std::string& instance);

    // template<>
    // Json Serializer::Write(const Reflection::object& instance);
    // template<>
    // Reflection::object& Serializer::Read(const Json& json_context, Reflection::object& instance);

    ////////////////////////////////////
    ////sample of generation coder
    ////////////////////////////////////
    // class test_class
    //{
    // public:
    //     int a;
    //     unsigned int b;
    //     std::vector<int> c;
    // };
    // class ss;
    // class jkj;
    // template<>
    // Json Serializer::Write(const ss& instance);
    // template<>
    // Json Serializer::Write(const jkj& instance);

    /*REFLECTION_TYPE(jkj)
    CLASS(jkj,Fields)
    {
        REFLECTION_BODY(jkj);
        int jl;
    };

    REFLECTION_TYPE(ss)
    CLASS(ss:public jkj,WhiteListFields)
    {
        REFLECTION_BODY(ss);
        int jl;
    };*/

    ////////////////////////////////////
    ////template of generation coder
    ////////////////////////////////////
    // template<>
    // Json Serializer::Write(const test_class& instance);
    // template<>
    // test_class& Serializer::Read(const Json& json_context, test_class& instance);

    //
    ////////////////////////////////////
} // namespace LittleEngine
