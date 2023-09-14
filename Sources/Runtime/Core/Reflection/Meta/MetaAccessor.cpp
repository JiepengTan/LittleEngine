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
        m_metaData = m_functions->GetMetaData();
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

    bool FieldAccessor::HasMeta(std::string metaKey)
    {
        return m_metaData.count(metaKey) != 0;
    }

    std::string FieldAccessor::GetMeta(std::string metaKey)
    {
        if(m_metaData.count(metaKey) == 0) return "";
        return m_metaData.at(metaKey);
    }
    bool FieldAccessor::IsArrayType()
    {
        // todo: should check validation
        return m_functions->IsArray();
    }

    bool FieldAccessor::IsEnum()
    {
        return m_functions->IsEnum();
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
        m_metaData        = dest.m_metaData;
        return *this;
    }

    MethodAccessor::MethodAccessor()
    {
        m_methodName = k_unknown;
        m_functions   = nullptr;
    }

    std::vector<std::string> MethodAccessor::GetParamsTypeName() const
    {
        return m_functions->GetFuncParamsType();
    }

    std::string MethodAccessor::GetReturnTypeName() const
    {
        return m_functions->GetFuncReturnType();
    }

    void MethodAccessor::Invoke(void* returnValue, void* instance, void* _1, void* _2, void* _3, void* _4, void* _5,
                                void* _6)
    {
        m_functions->GenericInvoke_Return_Instance_6Params(returnValue,instance,_1,_2,_3,_4,_5,_6);
    }

    MethodAccessor::MethodAccessor(MethodFunctionTuple* functions) : m_functions(functions)
    {
        m_methodName      = k_unknown;
        if (m_functions == nullptr)
        {
            return;
        }

        m_methodName      = m_functions->GetMethodName();
        m_metaData = m_functions->GetMetaData();
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
        m_metaData        = dest.m_metaData;
        return *this;
    }

    bool MethodAccessor::HasMeta(std::string metaKey)
    {
        return m_metaData.count(metaKey) != 0;
    }

    std::string MethodAccessor::GetMeta(std::string metaKey)
    {
        if(m_metaData.count(metaKey) == 0) return "";
        return m_metaData.at(metaKey);
    }
    
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
        m_metaData = m_func->GetMetaData();
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
        m_metaData        = dest.m_metaData;
        return *this;
    }
}
