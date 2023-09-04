// copy from https://github.com/BoomingTech/Piccolo
#include "Reflection.h"
#include <cstring>
#include <map>

namespace OvCore
{
    namespace Reflection
    {
        const char* k_unknown_type = "UnknownType";
        const char* k_unknown      = "Unknown";

        static std::map<std::string, ClassFunctionTuple*>       m_classMap;
        static std::multimap<std::string, FieldFunctionTuple*>  m_fieldMap;
        static std::multimap<std::string, MethodFunctionTuple*> m_methodMap;
        static std::map<std::string, ArrayFunctionTuple*>       m_arrayMap;

        void TypeMetaRegisterInterface::RegisterToFieldMap(const char* name, FieldFunctionTuple* value)
        {
            m_fieldMap.insert(std::make_pair(name, value));
        }
        void TypeMetaRegisterInterface::RegisterToMethodMap(const char* name, MethodFunctionTuple* value)
        {
            m_methodMap.insert(std::make_pair(name, value));
        }
        void TypeMetaRegisterInterface::RegisterToArrayMap(const char* name, ArrayFunctionTuple* value)
        {
            if (m_arrayMap.find(name) == m_arrayMap.end())
            {
                m_arrayMap.insert(std::make_pair(name, value));
            }
            else
            {
                delete value;
            }
        }

        void TypeMetaRegisterInterface::RegisterToClassMap(const char* name, ClassFunctionTuple* value)
        {
            if (m_classMap.find(name) == m_classMap.end())
            {
                m_classMap.insert(std::make_pair(name, value));
            }
            else
            {
                delete value;
            }
        }

        void TypeMetaRegisterInterface::UnRegisterAll()
        {
            for (const auto& itr : m_fieldMap)
            {
                delete itr.second;
            }
            m_fieldMap.clear();
            for (const auto& itr : m_classMap)
            {
                delete itr.second;
            }
            m_classMap.clear();
            for (const auto& itr : m_arrayMap)
            {
                delete itr.second;
            }
            m_arrayMap.clear();
        }

        TypeMeta::TypeMeta(std::string type_name) : m_typeName(type_name)
        {
            m_isValid = false;
            m_fields.clear();
            m_methods.clear();

            auto fileds_iter = m_fieldMap.equal_range(type_name);
            while (fileds_iter.first != fileds_iter.second)
            {
                FieldAccessor f_field(fileds_iter.first->second);
                m_fields.emplace_back(f_field);
                m_isValid = true;

                ++fileds_iter.first;
            }

            auto methods_iter = m_methodMap.equal_range(type_name);
            while (methods_iter.first != methods_iter.second)
            {
                MethodAccessor f_method(methods_iter.first->second);
                m_methods.emplace_back(f_method);
                m_isValid = true;

                ++methods_iter.first;
            }
        }

        TypeMeta::TypeMeta() : m_typeName(k_unknown_type), m_isValid(false) { m_fields.clear(); m_methods.clear(); }

        TypeMeta TypeMeta::NewMetaFromName(std::string type_name)
        {
            TypeMeta f_type(type_name);
            return f_type;
        }

        bool TypeMeta::NewArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor)
        {
            auto iter = m_arrayMap.find(array_type_name);

            if (iter != m_arrayMap.end())
            {
                ArrayAccessor New_accessor(iter->second);
                accessor = New_accessor;
                return true;
            }

            return false;
        }

        ReflectionInstance TypeMeta::NewFromNameAndJson(std::string type_name, const Json& json_context)
        {
            auto iter = m_classMap.find(type_name);

            if (iter != m_classMap.end())
            {
                return ReflectionInstance(TypeMeta(type_name), (std::get<1>(*iter->second)(json_context)));
            }
            return ReflectionInstance();
        }

        Json TypeMeta::WriteByName(std::string type_name, void* instance)
        {
            auto iter = m_classMap.find(type_name);

            if (iter != m_classMap.end())
            {
                return std::get<2>(*iter->second)(instance);
            }
            return Json();
        }

        std::string TypeMeta::GetTypeName() { return m_typeName; }

        int TypeMeta::GetFieldsList(FieldAccessor*& out_list)
        {
            int count = m_fields.size();
            out_list  = new FieldAccessor[count];
            for (int i = 0; i < count; ++i)
            {
                out_list[i] = m_fields[i];
            }
            return count;
        }

        int TypeMeta::GetMethodsList(MethodAccessor*& out_list)
        {
            int count = m_methods.size();
            out_list  = new MethodAccessor[count];
            for (int i = 0; i < count; ++i)
            {
                out_list[i] = m_methods[i];
            }
            return count;
        }

        int TypeMeta::GetBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance)
        {
            auto iter = m_classMap.find(m_typeName);

            if (iter != m_classMap.end())
            {
                return (std::get<0>(*iter->second))(out_list, instance);
            }

            return 0;
        }

        FieldAccessor TypeMeta::GetFieldByName(const char* name)
        {
            const auto it = std::find_if(m_fields.begin(), m_fields.end(), [&](const auto& i) {
                return std::strcmp(i.GetFieldName(), name) == 0;
            });
            if (it != m_fields.end())
                return *it;
            return FieldAccessor(nullptr);
        }

        MethodAccessor TypeMeta::GetMethodByName(const char* name)
        {
            const auto it = std::find_if(m_methods.begin(), m_methods.end(), [&](const auto& i) {
                return std::strcmp(i.GetMethodName(), name) == 0;
            });
            if (it != m_methods.end())
                return *it;
            return MethodAccessor(nullptr);
        }

        TypeMeta& TypeMeta::operator=(const TypeMeta& dest)
        {
            if (this == &dest)
            {
                return *this;
            }
            m_fields.clear();
            m_fields = dest.m_fields;

            
            m_methods.clear();
            m_methods = dest.m_methods;

            m_typeName = dest.m_typeName;
            m_isValid  = dest.m_isValid;

            return *this;
        }
        FieldAccessor::FieldAccessor()
        {
            m_fieldTypeName = k_unknown_type;
            m_fieldName      = k_unknown;
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

            m_fieldTypeName = (std::get<4>(*m_functions))();
            m_fieldName      = (std::get<3>(*m_functions))();
        }

        void* FieldAccessor::Get(void* instance)
        {
            // todo: should check validation
            return static_cast<void*>((std::get<1>(*m_functions))(instance));
        }

        void FieldAccessor::Set(void* instance, void* value)
        {
            // todo: should check validation
            (std::get<0>(*m_functions))(instance, value);
        }

        TypeMeta FieldAccessor::GetOwnerTypeMeta()
        {
            // todo: should check validation
            TypeMeta f_type((std::get<2>(*m_functions))());
            return f_type;
        }

        bool FieldAccessor::GetTypeMeta(TypeMeta& field_type)
        {
            TypeMeta f_type(m_fieldTypeName);
            field_type = f_type;
            return f_type.m_isValid;
        }

        const char* FieldAccessor::GetFieldName() const { return m_fieldName; }
        const char* FieldAccessor::GetFieldTypeName() { return m_fieldTypeName; }

        bool FieldAccessor::IsArrayType()
        {
            // todo: should check validation
            return (std::get<5>(*m_functions))();
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

            m_methodName      = (std::get<0>(*m_functions))();
        }
        const char* MethodAccessor::GetMethodName() const{
            return (std::get<0>(*m_functions))();
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
        void MethodAccessor::Invoke(void* instance) { (std::get<1>(*m_functions))(instance); }
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

            m_arrayTypeName   = std::get<3>(*m_func)();
            m_elementTypeName = std::get<4>(*m_func)();
        }
        const char* ArrayAccessor::GetArrayTypeName() { return m_arrayTypeName; }
        const char* ArrayAccessor::GetElementTypeName() { return m_elementTypeName; }
        void        ArrayAccessor::Set(int index, void* instance, void* element_value)
        {
            // todo: should check validation
            size_t count = GetSize(instance);
            // todo: should check validation(index < count)
            std::get<0> (*m_func)(index, instance, element_value);
        }

        void* ArrayAccessor::Get(int index, void* instance)
        {
            // todo: should check validation
            size_t count = GetSize(instance);
            // todo: should check validation(index < count)
            return std::get<1>(*m_func)(index, instance);
        }

        int ArrayAccessor::GetSize(void* instance)
        {
            // todo: should check validation
            return std::get<2>(*m_func)(instance);
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

        ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance& dest)
        {
            if (this == &dest)
            {
                return *this;
            }
            m_instance = dest.m_instance;
            m_meta     = dest.m_meta;

            return *this;
        }

        ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance&& dest)
        {
            if (this == &dest)
            {
                return *this;
            }
            m_instance = dest.m_instance;
            m_meta     = dest.m_meta;

            return *this;
        }
    } // namespace Reflection
} // namespace LittleEngine