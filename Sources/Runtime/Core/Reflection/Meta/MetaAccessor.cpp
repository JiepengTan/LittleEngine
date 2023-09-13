#include "MetaAccessor.h"

#include "MetaRegisterUtil.h"
#include "TypeInfo.h"

namespace LittleEngine::Reflection
{
    FieldAccessor::FieldAccessor()
    {
        m_fieldTypeName   = k_unknown_type;
        m_fieldName       = k_unknown;
        m_functions       = nullptr;
    }

    FieldAccessor::FieldAccessor(FieldFunctionTuple* functions) : m_functions(functions)
    {
        m_fieldTypeName = k_unknown_type;
        m_fieldName      = k_unknown;
        if (m_functions == nullptr)
        {
            return;
        }

        m_fieldTypeName = m_functions->GetFieldType();
        m_fieldName      = m_functions->GetFiledName();
    }

    void* FieldAccessor::Get(void* instance)
    {
        return m_functions->Get(instance);
    }

    void FieldAccessor::Set(void* instance, void* value)
    {
        // todo: should check validation
        m_functions->Set(instance, value);
    }

    TypeInfo* FieldAccessor::GetOwnerTypeInfo()
    {
        // todo: should check validation
        return TypeInfo::GetType(m_functions->GetClassName());
    }

    TypeInfo* FieldAccessor::GetType()
    {
        return TypeInfo::GetType(m_fieldTypeName);
    }

    bool FieldAccessor::HasTypeInfo()
    {
        return TypeInfo::GetType(m_fieldTypeName) != nullptr;
    }

    std::string FieldAccessor::GetFieldName() const { return m_fieldName; }
    std::string FieldAccessor::GetFieldTypeName() { return m_fieldTypeName; }

    bool FieldAccessor::IsArrayType()
    {
        // todo: should check validation
        return m_functions->IsArray();
    }

    FieldAccessor& FieldAccessor::operator=(const FieldAccessor& dest)
    {
        if (this == &dest)
        {
            return *this;
        }
        m_functions       = dest.m_functions;
        m_fieldName      = dest.m_fieldName;
        m_fieldTypeName = dest.m_fieldTypeName;
        return *this;
    }

    MethodAccessor::MethodAccessor()
    {
        m_methodName = k_unknown;
        m_functions   = nullptr;
    }

    MethodAccessor::MethodAccessor(MethodFunctionTuple* functions) : m_functions(functions)
    {
        m_methodName      = k_unknown;
        if (m_functions == nullptr)
        {
            return;
        }

        m_methodName      = m_functions->GetMethodName();
    }
    std::string MethodAccessor::GetMethodName() const{
        return m_methodName;
    }
    MethodAccessor& MethodAccessor::operator=(const MethodAccessor& dest)
    {
        if (this == &dest)
        {
            return *this;
        }
        m_functions       = dest.m_functions;
        m_methodName      = dest.m_methodName;
        return *this;
    }
    void MethodAccessor::Invoke(void* instance) { m_functions->Invoke(instance); }
    ArrayAccessor::ArrayAccessor() :
        m_func(nullptr), m_arrayTypeName("UnKnownType"), m_elementTypeName("UnKnownType")
    {}

    ArrayAccessor::ArrayAccessor(ArrayFunctionTuple* array_func) : m_func(array_func)
    {
        m_arrayTypeName   = k_unknown_type;
        m_elementTypeName = k_unknown_type;
        if (m_func == nullptr)
        {
            return;
        }

        m_arrayTypeName   =m_func->GetArrayTypeName();
        m_elementTypeName =m_func->GetElementTypeName();
    }
    std::string ArrayAccessor::GetArrayTypeName() { return m_arrayTypeName; }
    std::string ArrayAccessor::GetElementTypeName() { return m_elementTypeName; }
    void        ArrayAccessor::Set(int index, void* instance, void* element_value)
    {
        // todo: should check validation
        size_t count = GetSize(instance);
        // todo: should check validation(index < count)
        m_func->Set (index, instance, element_value);
    }

    void* ArrayAccessor::Get(int index, void* instance)
    {
        // todo: should check validation
        size_t count = GetSize(instance);
        // todo: should check validation(index < count)
        return m_func->Get(index, instance);
    }

    int ArrayAccessor::GetSize(void* instance)
    {
        // todo: should check validation
        return m_func->GetSize(instance);
    }

    ArrayAccessor& ArrayAccessor::operator=(ArrayAccessor& dest)
    {
        if (this == &dest)
        {
            return *this;
        }
        m_func              = dest.m_func;
        m_arrayTypeName   = dest.m_arrayTypeName;
        m_elementTypeName = dest.m_elementTypeName;
        return *this;
    }
}
