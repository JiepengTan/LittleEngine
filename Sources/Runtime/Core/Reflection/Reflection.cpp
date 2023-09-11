// copy from https://github.com/BoomingTech/Piccolo
#include "Reflection.h"
#include "cstring"
#include "map"

#include "Core/Base/Macro.h"

namespace LittleEngine
{
    namespace Reflection
    {
        const char* k_unknown_type = "UnknownType";
        const char* k_unknown      = "Unknown";

        
        static std::map<TypeID, std::string>                    m_id2ClassNameMap;
        static std::map<std::string, ClassFunctionTuple*>       m_classMap;
        static std::multimap<std::string, FieldFunctionTuple*>  m_fieldMap;
        static std::multimap<std::string, MethodFunctionTuple*> m_methodMap;
        static std::map<std::string, ArrayFunctionTuple*>       m_arrayMap;

        
        std::map<TypeID,TypeMeta*> TypeMeta::m_id2Types;
        std::map<std::string,TypeMeta*> TypeMeta::m_name2Types;
        std::map<TypeID,std::vector<TypeMeta*>> TypeMeta::m_id2BaseClassTypes;
        
        TypeMeta* TypeMeta::GetType(std::string type_name)
        {
            if(m_name2Types.count(type_name) == 0) return nullptr;
            return m_name2Types.at(type_name);
        }
        TypeMeta* TypeMeta::GetType(TypeID typeId)
        {
            if(m_id2Types.count(typeId) == 0) return nullptr;
            return m_id2Types.at(typeId);
        }
        void GetBaseClass()
        {
            
        }
        
        void TypeMeta::RecvGetSuperClassMetas(std::string type_name,std::vector<TypeMeta*>& supperClasses)
        {
            auto ids = GetBaseClassIds(type_name);
            for (auto id : ids)
            {
                if(m_id2Types.count(id) == 0)
                {
                    auto className= m_id2ClassNameMap.at(id);
                    auto base= RegisterType(className,id);
                    supperClasses.push_back(base);
                }else
                {
                    supperClasses.push_back(m_id2Types.at(id));
                }
                RecvGetSuperClassMetas(m_id2ClassNameMap.at(id),supperClasses);
            }
        }
        TypeMeta* TypeMeta::RegisterType(std::string type_name,TypeID typeId)
        {
            if(m_id2Types.count(typeId) !=0) return m_id2Types.at(typeId);
            TypeMeta* meta = new TypeMeta(type_name,typeId);
            m_name2Types.emplace(type_name,meta);
            m_id2Types.emplace(typeId,meta);
            std::vector<TypeMeta*> baseClasses;
            RecvGetSuperClassMetas(type_name,baseClasses);
            m_id2BaseClassTypes[typeId] = baseClasses;
            return meta;
        }
        
        void TypeMeta::Clear()
        {
            for (auto item : m_id2Types)
            {
                delete item.second;
            }
            m_name2Types.clear();
            m_id2Types.clear();
        }

        
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

        std::string TypeMetaRegisterInterface::GetTypeName(TypeID typeId)
        {
            if (m_id2ClassNameMap.find(typeId) != m_id2ClassNameMap.end())
            {
                return m_id2ClassNameMap.at(typeId);
            }
            return k_unknown_type;
        }

        void TypeMetaRegisterInterface::RegisterToClassMap(const char* name, ClassFunctionTuple* value, TypeID typeId)
        {
            if (m_id2ClassNameMap.find(typeId) == m_id2ClassNameMap.end())
            {
                m_id2ClassNameMap.insert(std::make_pair(typeId, std::string(name)));
            }
            
            if (m_classMap.find(name) == m_classMap.end())
            {
                m_id2ClassNameMap.insert(std::make_pair(typeId, std::string(name)));
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

        TypeMeta::TypeMeta(std::string type_name,TypeID typeId) : m_typeName(type_name)
        {
            m_isValid = false;
            m_fields.clear();
            m_methods.clear();
            m_typeId = typeId;
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

        TypeMeta::TypeMeta() : m_typeName(k_unknown_type), m_isValid(false), m_typeId(0)
        {
            m_fields.clear();
            m_methods.clear();
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
        void* TypeMeta::CreateFromNameAndJson(std::string type_name, const Json& json_context)
        {
            auto iter = m_classMap.find(type_name);
            if (iter != m_classMap.end())
            {
                return (iter->second->ConstructorWithJsonFunc(json_context));
            }
            return nullptr;
        }
        
        ReflectionInstance TypeMeta::NewFromNameAndJson(std::string type_name, const Json& json_context)
        {
            auto iter = m_classMap.find(type_name);
            if (iter != m_classMap.end())
            {
                return ReflectionInstance(GetType(type_name), ( iter->second->ConstructorWithJsonFunc(json_context)));
            }
            return ReflectionInstance();
        }

        Json TypeMeta::WriteByName(std::string type_name, void* instance)
        {
            auto iter = m_classMap.find(type_name);

            if (iter != m_classMap.end())
            {
                return iter->second->WriteJsonByNameFunc(instance);
            }
            return Json();
        }

        std::string TypeMeta::GetTypeName() { return m_typeName; }

        TypeID TypeMeta::GetTypeID() const { return m_typeId;}

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

        
        std::vector<TypeID> TypeMeta::GetBaseClassIds(std::string type_name){
            auto iter = m_classMap.find(type_name);
            if (iter != m_classMap.end())
            {
                return iter->second->GetBaseClassIds();
            }
            return std::vector<TypeID>();
        }
                
        int TypeMeta::GetBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance)
        {
            auto iter = m_classMap.find(m_typeName);

            if (iter != m_classMap.end())
            {
                return iter->second->GetBaseClassReflectionInstanceList(out_list, instance);
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

        bool TypeMeta::IsSubclassOf(TypeID typeId)
        {
            if(m_id2BaseClassTypes.count(typeId) == 0) return false;
            auto& suppers = m_id2BaseClassTypes.at(typeId);
            for (auto supper : suppers)
            {
                if(supper->m_typeId == typeId) return true;
            }
            return false;
        }

        bool TypeMeta::IsAssignableFrom(TypeID typeId)
        {
            return typeId == m_typeId || IsSubclassOf(typeId);
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

            m_fieldTypeName = m_functions->GetFieldType();
            m_fieldName      = m_functions->GetFiledName();
        }

        void* FieldAccessor::Get(void* instance)
        {
            // todo: should check validation
            return static_cast<void*>(m_functions->Get(instance));
        }

        void FieldAccessor::Set(void* instance, void* value)
        {
            // todo: should check validation
            m_functions->Set(instance, value);
        }

        TypeMeta* FieldAccessor::GetOwnerTypeMeta()
        {
            // todo: should check validation
            return TypeMeta::GetType(m_functions->GetClassName());
        }

        bool FieldAccessor::HasTypeMeta()
        {
            return TypeMeta::GetType(m_fieldTypeName) == nullptr;
        }

        const char* FieldAccessor::GetFieldName() const { return m_fieldName; }
        const char* FieldAccessor::GetFieldTypeName() { return m_fieldTypeName; }

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
        const char* MethodAccessor::GetMethodName() const{
            return m_functions->GetMethodName();
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
        const char* ArrayAccessor::GetArrayTypeName() { return m_arrayTypeName; }
        const char* ArrayAccessor::GetElementTypeName() { return m_elementTypeName; }
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
