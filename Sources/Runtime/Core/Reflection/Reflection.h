#pragma once
// copy from https://github.com/BoomingTech/Piccolo
#include "Core/Serialization/Json.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_Set>
#include <vector>

namespace OvCore
{

#if defined(__REFLECTION_PARSER__)
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
#define STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name
//#define CLASS(class_name,...) class __attribute__((annotate(#__VA_ARGS__))) class_name:public Reflection::object
#else
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name
//#define CLASS(class_name,...) class class_name:public Reflection::object
#endif // __REFLECTION_PARSER__

#define REFLECTION_BODY(class_name) \
    friend class Reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
    friend class JsonSerializer;
    // public: virtual std::string GetTypeName() override {return #class_name;}

#define REFLECTION_TYPE(class_name) \
    namespace Reflection \
    { \
        namespace TypeFieldReflectionOparator \
        { \
            class Type##class_name##Operator; \
        } \
    };

#define REGISTER_FIELD_TO_MAP(name, value) OvCore::Reflection::TypeMetaRegisterInterface::RegisterToFieldMap(name, value);
#define REGISTER_Method_TO_MAP(name, value) OvCore::Reflection::TypeMetaRegisterInterface::RegisterToMethodMap(name, value);
#define REGISTER_BASE_CLASS_TO_MAP(name, value) OvCore::Reflection::TypeMetaRegisterInterface::RegisterToClassMap(name, value);
#define REGISTER_ARRAY_TO_MAP(name, value) OvCore::Reflection::TypeMetaRegisterInterface::RegisterToArrayMap(name, value);
#define UNREGISTER_ALL OvCore::Reflection::TypeMetaRegisterInterface::UnRegisterAll();

#define PICCOLO_REFLECTION_NEW(name, ...) OvCore::Reflection::ReflectionPtr(#name, new name(__VA_ARGS__));
#define PICCOLO_REFLECTION_DELETE(value) \
    if (value) \
    { \
        delete value.operator->(); \
        value.GetPtrReference() = nullptr; \
    }
#define PICCOLO_REFLECTION_DEEP_COPY(type, dst_ptr, src_ptr) \
    *static_cast<type*>(dst_ptr) = *static_cast<type*>(src_ptr.GetPtr());

#define TypeMetaDef(class_name, ptr) \
    OvCore::Reflection::ReflectionInstance(OvCore::Reflection::TypeMeta::NewMetaFromName(#class_name), \
                                            (class_name*)ptr)

#define TypeMetaDefPtr(class_name, ptr) \
    new OvCore::Reflection::ReflectionInstance(OvCore::Reflection::TypeMeta::NewMetaFromName(#class_name), \
                                                (class_name*)ptr)

    template<typename T, typename U, typename = void>
    struct is_safely_castable : std::false_type
    {};

    template<typename T, typename U>
    struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>()))>> : std::true_type
    {};

    namespace Reflection
    {
        class TypeMeta;
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

    typedef std::function<void*(const Json&)>                           ConstructorWithJson;
    typedef std::function<Json(void*)>                                  WriteJsonByName;
    typedef std::function<int(Reflection::ReflectionInstance*&, void*)> GetBaseClassReflectionInstanceListFunc;

    typedef std::tuple<SetFunction, GetFunction, GetNameFunction, GetNameFunction, GetNameFunction, GetBoolFunc>
                                                       FieldFunctionTuple;
    typedef std::tuple<GetNameFunction, InvokeFunction> MethodFunctionTuple;
    typedef std::tuple<GetBaseClassReflectionInstanceListFunc, ConstructorWithJson, WriteJsonByName> ClassFunctionTuple;
    typedef std::tuple<SetArrayFunc, GetArrayFunc, GetSizeFunc, GetNameFunction, GetNameFunction>      ArrayFunctionTuple;

    namespace Reflection
    {
        class TypeMetaRegisterInterface
        {
        public:
            static void RegisterToClassMap(const char* name, ClassFunctionTuple* value);
            static void RegisterToFieldMap(const char* name, FieldFunctionTuple* value);

            static void RegisterToMethodMap(const char* name, MethodFunctionTuple* value);
            static void RegisterToArrayMap(const char* name, ArrayFunctionTuple* value);

            static void UnRegisterAll();
        };
        class TypeMeta
        {
            friend class FieldAccessor;
            friend class ArrayAccessor;
            friend class TypeMetaRegisterInterface;

        public:
            TypeMeta();

            // static void Register();

            static TypeMeta NewMetaFromName(std::string type_name);

            static bool               NewArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor);
            static ReflectionInstance NewFromNameAndJson(std::string type_name, const Json& json_context);
            static Json               WriteByName(std::string type_name, void* instance);

            std::string GetTypeName();

            int GetFieldsList(FieldAccessor*& out_list);
            int GetMethodsList(MethodAccessor*& out_list);

            int GetBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance);

            FieldAccessor GetFieldByName(const char* name);
            MethodAccessor GetMethodByName(const char* name);

            bool IsValid() { return m_isValid; }

            TypeMeta& operator=(const TypeMeta& dest);

        private:
            TypeMeta(std::string type_name);

        private:
            std::vector<FieldAccessor, std::allocator<FieldAccessor>>   m_fields;
            std::vector<MethodAccessor, std::allocator<MethodAccessor>> m_methods;
            std::string                                                 m_typeName;

            bool m_isValid;
        };

        class FieldAccessor
        {
            friend class TypeMeta;

        public:
            FieldAccessor();

            void* Get(void* instance);
            void  Set(void* instance, void* value);

            TypeMeta GetOwnerTypeMeta();

            /**
             * param: TypeMeta out_type
             *        a reference of TypeMeta
             *
             * return: bool value
             *        true: it's a reflection type
             *        false: it's not a reflection type
             */
            bool        GetTypeMeta(TypeMeta& field_type);
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
            friend class TypeMeta;

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
            friend class TypeMeta;

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

        class ReflectionInstance
        {
        public:
            ReflectionInstance(TypeMeta Meta, void* instance) : m_meta(Meta), m_instance(instance) {}
            ReflectionInstance() : m_meta(), m_instance(nullptr) {}

            ReflectionInstance& operator=(ReflectionInstance& dest);

            ReflectionInstance& operator=(ReflectionInstance&& dest);

        public:
            TypeMeta m_meta;
            void*    m_instance;
        };

        template<typename T>
        class ReflectionPtr
        {
            template<typename U>
            friend class ReflectionPtr;

        public:
            ReflectionPtr(std::string type_name, T* instance) : m_typeName(type_name), m_instance(instance) {}
            ReflectionPtr() : m_typeName(), m_instance(nullptr) {}

            ReflectionPtr(const ReflectionPtr& dest) : m_typeName(dest.m_typeName), m_instance(dest.m_instance) {}

            template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type */>
            ReflectionPtr<T>& operator=(const ReflectionPtr<U>& dest)
            {
                if (this == static_cast<void*>(&dest))
                {
                    return *this;
                }
                m_typeName = dest.m_typeName;
                m_instance  = static_cast<T*>(dest.m_instance);
                return *this;
            }

            template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
            ReflectionPtr<T>& operator=(ReflectionPtr<U>&& dest)
            {
                if (this == static_cast<void*>(&dest))
                {
                    return *this;
                }
                m_typeName = dest.m_typeName;
                m_instance  = static_cast<T*>(dest.m_instance);
                return *this;
            }

            ReflectionPtr<T>& operator=(const ReflectionPtr<T>& dest)
            {
                if (this == &dest)
                {
                    return *this;
                }
                m_typeName = dest.m_typeName;
                m_instance  = dest.m_instance;
                return *this;
            }

            ReflectionPtr<T>& operator=(ReflectionPtr<T>&& dest)
            {
                if (this == &dest)
                {
                    return *this;
                }
                m_typeName = dest.m_typeName;
                m_instance  = dest.m_instance;
                return *this;
            }

            std::string GetTypeName() const { return m_typeName; }

            void SetTypeName(std::string name) { m_typeName = name; }

            bool operator==(const T* ptr) const { return (m_instance == ptr); }

            bool operator!=(const T* ptr) const { return (m_instance != ptr); }

            bool operator==(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }

            bool operator!=(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

            template<
                typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
            explicit operator T1*()
            {
                return static_cast<T1*>(m_instance);
            }

            template<
                typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
            operator ReflectionPtr<T1>()
            {
                return ReflectionPtr<T1>(m_typeName, (T1*)(m_instance));
            }

            template<
                typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
            explicit operator const T1*() const
            {
                return static_cast<T1*>(m_instance);
            }

            template<
                typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
            operator const ReflectionPtr<T1>() const
            {
                return ReflectionPtr<T1>(m_typeName, (T1*)(m_instance));
            }

            T* operator->() { return m_instance; }

            T* operator->() const { return m_instance; }

            T& operator*() { return *(m_instance); }

            T* GetPtr() { return m_instance; }

            T* GetPtr() const { return m_instance; }

            const T& operator*() const { return *(static_cast<const T*>(m_instance)); }

            T*& GetPtrReference() { return m_instance; }

            operator bool() const { return (m_instance != nullptr); }

        private:
            std::string m_typeName {""};
            typedef T   m_type;
            T*          m_instance {nullptr};
        };

    } // namespace Reflection

} // namespace LittleEngine
