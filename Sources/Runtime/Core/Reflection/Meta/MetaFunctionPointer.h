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
    typedef std::function<std::string()>           GetNameFunction;
    typedef std::function<void(int, void*, void*)> SetArrayFunc;
    typedef std::function<void*(int, void*)>       GetArrayFunc;
    typedef std::function<int(void*)>              GetSizeFunc;
    typedef std::function<bool()>                  GetBoolFunc;
    typedef std::function<void(void*)>             InvokeFunction;

    
    typedef std::function<void*(const Json&)>                           ConstructorWithJsonFunc;
    typedef std::function<Json(void*)>                                  WriteJsonByNameFunc;
    typedef std::function<std::vector<TypeID>(void)>                    GetBaseClassIdsFunc;
    typedef std::function<void*(void)>                                  ConstructorFunc;
    typedef std::function<std::map<std::string,std::string>(void)>      GetMetaDataFunc;
    
    //(void* retValPtr, void* instance, void* _1,void* _2 ,
    //      void* _3,void* _4,void* _5 ,void* _6  )
    typedef std::function<void(void* ,void* ,void* ,void* ,
                               void* ,void* ,void* ,void* )>            GenericInvokeFunction;
    typedef std::function<std::string(void)>                            GetFuncReturnTypeFunc;
    typedef std::function<std::vector<std::string>(void)>               GetFuncParamsTypeFunc;
    
    class FieldFunctionTuple
    {
    public:
        SetFunction Set;
        GetFunction Get;
        GetNameFunction GetClassName;
        GetNameFunction GetFiledName;
        GetNameFunction GetFieldType;
        GetBoolFunc IsArray;
        GetMetaDataFunc GetMetaData;
    };
    class MethodFunctionTuple
    {
    public:
        GetNameFunction GetMethodName;
        GetMetaDataFunc GetMetaData;
        GenericInvokeFunction GenericInvoke_Return_Instance_6Params;
        GetFuncReturnTypeFunc GetFuncReturnType;
        GetFuncParamsTypeFunc GetFuncParamsType;
    };
    class ClassFunctionTuple
    {
    public:
        ConstructorWithJsonFunc ConstructorWithJson;
        WriteJsonByNameFunc WriteJsonByName;
        GetBaseClassIdsFunc GetBaseClassIds;
        ConstructorFunc Constructor;
        GetBoolFunc    IsAbstract;
        GetMetaDataFunc    GetMetaData;
    };
    class ArrayFunctionTuple
    {
    public:
        SetArrayFunc Set;
        GetArrayFunc Get;
        GetSizeFunc GetSize;
        GetNameFunction GetArrayTypeName;
        GetNameFunction GetElementTypeName;
        GetMetaDataFunc    GetMetaData;
    };
}