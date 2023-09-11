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

        TypeInfo* GetOwnerTypeInfo();

        bool        HasTypeInfo();
        const char* GetFieldName() const;
        const char* GetFieldTypeName();
        bool        IsArrayType();

        FieldAccessor& operator=(const FieldAccessor& dest);

    private:
        FieldAccessor(FieldFunctionTuple* functions);

    private:
        FieldFunctionTuple* m_functions;
        const char*         m_fieldName;
        const char*         m_fieldTypeName;
    };
    class MethodAccessor
    {
        friend class TypeInfo;

    public:
        MethodAccessor();

        void Invoke(void* instance);

        const char* GetMethodName() const;

        MethodAccessor& operator=(const MethodAccessor& dest);

    private:
        MethodAccessor(MethodFunctionTuple* functions);

    private:
        MethodFunctionTuple* m_functions;
        const char*          m_methodName;
    };
    /**
     *  Function reflection is not implemented, so use this as an std::vector accessor
     */
    class ArrayAccessor
    {
        friend class TypeInfo;

    public:
        ArrayAccessor();
        const char* GetArrayTypeName();
        const char* GetElementTypeName();
        void        Set(int index, void* instance, void* element_value);

        void* Get(int index, void* instance);
        int   GetSize(void* instance);

        ArrayAccessor& operator=(ArrayAccessor& dest);

    private:
        ArrayAccessor(ArrayFunctionTuple* array_func);

    private:
        ArrayFunctionTuple* m_func;
        const char*         m_arrayTypeName;
        const char*         m_elementTypeName;
    };
}