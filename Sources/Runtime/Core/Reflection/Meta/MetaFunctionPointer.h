#pragma once
#include "MetaMacro.h"

namespace LittleEngine::Reflection
{

    template<typename T, typename U, typename = void>
    struct is_safely_castable : std::false_type
    {};

    template<typename T, typename U>
    struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>()))>> : std::true_type
    {};

    namespace Reflection
    {
        class TypeInfo;
        class FieldAccessor;
        class MethodAccessor;
        class ArrayAccessor;
        class ReflectionInstance;
    } // namespace Reflection
    typedef std::function<void(void*, void*)>      SetFunction;
    typedef std::function<void*(void*)>            GetFunction;
    typedef std::function<const char*()>           GetNameFunction;
    typedef std::function<void(int, void*, void*)> SetArrayFunc;
    typedef std::function<void*(int, void*)>       GetArrayFunc;
    typedef std::function<int(void*)>              GetSizeFunc;
    typedef std::function<bool()>                  GetBoolFunc;
    typedef std::function<void(void*)>             InvokeFunction;

    typedef std::function<void*(const Json&)>                           ConstructorWithJsonFunc;
    typedef std::function<Json(void*)>                                  WriteJsonByNameFunc;
    typedef std::function<std::vector<TypeID>(void)>                    GetBaseClassIdsFunc;
    typedef std::function<void*(void)>                                  ConstructorFunc;
    
    class FieldFunctionTuple
    {
    public:
        SetFunction Set;
        GetFunction Get;
        GetNameFunction GetClassName;
        GetNameFunction GetFiledName;
        GetNameFunction GetFieldType;
        GetBoolFunc IsArray;
    };
    class MethodFunctionTuple
    {
    public:
       GetNameFunction GetMethodName;
        InvokeFunction Invoke;
    };
    class ClassFunctionTuple
    {
    public:
        ConstructorWithJsonFunc ConstructorWithJson;
        WriteJsonByNameFunc WriteJsonByName;
        GetBaseClassIdsFunc GetBaseClassIds;
        ConstructorFunc Constructor;
    };
    class ArrayFunctionTuple
    {
    public:
        SetArrayFunc Set;
        GetArrayFunc Get;
        GetSizeFunc GetSize;
        GetNameFunction GetArrayTypeName;
        GetNameFunction GetElementTypeName;
    };
}