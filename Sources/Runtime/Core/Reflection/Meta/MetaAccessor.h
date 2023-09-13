#pragma once
#include "MetaFunctionPointer.h"

namespace LittleEngine::Reflection
{
    class FieldAccessor
    {
        friend class TypeInfo;

    public:
        FieldAccessor();
        void* Get(void* instance);
        void  Set(void* instance, void* value);

        TypeInfo*   GetOwnerTypeInfo();

        TypeInfo*   GetType();
        bool        HasTypeInfo();
        std::string GetFieldName() const;
        std::string GetFieldTypeName();
        bool        IsArrayType();

        bool HasMeta(std::string metaKey);
        std::string GetMeta(std::string metaKey);
        
        FieldAccessor& operator=(const FieldAccessor& dest);

    private:
        FieldAccessor(FieldFunctionTuple* functions);

    private:
        FieldFunctionTuple* m_functions;
        std::string         m_fieldName;
        std::string         m_fieldTypeName;
        std::map<std::string,std::string> m_metaData;
    };


    class MethodAccessor
    {
        friend class TypeInfo;

    public:
        MethodAccessor();

        std::vector<std::string> GetParamsTypeName() const;
        std::string GetReturnTypeName() const;
        void Invoke(void* retrunPtr,void* instance,void* _1= nullptr,void* _2= nullptr,void* _3= nullptr,void* _4= nullptr,void* _5= nullptr,void* _6 = nullptr);
        bool HasMeta(std::string metaKey);
        std::string GetMeta(std::string metaKey);

        std::string GetMethodName() const;

        MethodAccessor& operator=(const MethodAccessor& dest);

    private:
        MethodAccessor(MethodFunctionTuple* functions);

    private:
        MethodFunctionTuple* m_functions;
        std::string          m_methodName;
        std::map<std::string,std::string> m_metaData;
    };
    /**
     *  Function reflection is not implemented, so use this as an std::vector accessor
     */
    class ArrayAccessor
    {
        friend class TypeInfo;

    public:
        ArrayAccessor();
        std::string GetArrayTypeName();
        std::string GetElementTypeName();
        void        Set(int index, void* instance, void* element_value);

        void* Get(int index, void* instance);
        int   GetSize(void* instance);

        ArrayAccessor& operator=(ArrayAccessor& dest);

    private:
        ArrayAccessor(ArrayFunctionTuple* array_func);

    private:
        ArrayFunctionTuple* m_func;
        std::string         m_arrayTypeName;
        std::string         m_elementTypeName;
        std::map<std::string,std::string> m_metaData;
    };
}