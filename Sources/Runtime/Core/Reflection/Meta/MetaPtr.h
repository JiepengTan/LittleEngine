#pragma once
#include <string>

namespace LittleEngine::Reflection
{
    template<typename T>
     class MetaPtr
    {
        template<typename U>
        friend class MetaPtr;

    public:
        MetaPtr(std::string type_name, T* instance) : m_typeName(type_name), m_instance(instance) {}
        MetaPtr() : m_typeName(), m_instance(nullptr) {}

        MetaPtr(const MetaPtr& dest) : m_typeName(dest.m_typeName), m_instance(dest.m_instance) {}

        template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type */>
        MetaPtr<T>& operator=(const MetaPtr<U>& dest)
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
        MetaPtr<T>& operator=(MetaPtr<U>&& dest)
        {
            if (this == static_cast<void*>(&dest))
            {
                return *this;
            }
            m_typeName = dest.m_typeName;
            m_instance  = static_cast<T*>(dest.m_instance);
            return *this;
        }

        MetaPtr<T>& operator=(const MetaPtr<T>& dest)
        {
            if (this == &dest)
            {
                return *this;
            }
            m_typeName = dest.m_typeName;
            m_instance  = dest.m_instance;
            return *this;
        }

        MetaPtr<T>& operator=(MetaPtr<T>&& dest)
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

        bool operator==(const MetaPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }

        bool operator!=(const MetaPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        explicit operator T1*()
        {
            return static_cast<T1*>(m_instance);
        }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        operator MetaPtr<T1>()
        {
            return MetaPtr<T1>(m_typeName, (T1*)(m_instance));
        }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        explicit operator const T1*() const
        {
            return static_cast<T1*>(m_instance);
        }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        operator const MetaPtr<T1>() const
        {
            return MetaPtr<T1>(m_typeName, (T1*)(m_instance));
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
}
