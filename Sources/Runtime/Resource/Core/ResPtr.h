#pragma once
#include "Core/Reflection/Reflection.h"
#include "Resource/Data/ResIncludeBasic.h"
#include "Resource/Core/ResourcesUtils.h"

namespace LittleEngine
{
    enum class EResType
    {
        EResInvalid=0,
        EResMesh = 1,
        EResShader = 2,
        EResTexture= 3,
        EResModel = 4,
        EResAnimation = 5,
        EResSound = 6,
        EResMaterial = 7,
    };

    template<typename T>
    class ResPtr
    {
        template<typename U>
        friend class ResPtr;
    public:
        ResPtr(EResType type,StringText guid, T* instance) :m_guid(guid), m_resType(type), m_instance(instance) {}
        ResPtr() : m_resType(), m_instance(nullptr) {}

        ResPtr(const ResPtr& dest) : m_resType(dest.m_resType), m_instance(dest.m_instance) {}
        void Reset(StringText guid, T* instance)
        {
            m_guid = guid;
            m_instance = instance;
        }
        template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type */>
        ResPtr<T>& operator=(const ResPtr<U>& dest)
        {
            if (this == static_cast<void*>(&dest))
            {
                return *this;
            }
            m_resType = dest.m_resType;
            m_guid = dest.m_guid;
            m_instance  = static_cast<T*>(dest.m_instance);
            return *this;
        }

        template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
        ResPtr<T>& operator=(ResPtr<U>&& dest)
        {
            if (this == static_cast<void*>(&dest))
            {
                return *this;
            }
            m_resType = dest.m_resType;
            m_guid = dest.m_guid;
            m_instance  = static_cast<T*>(dest.m_instance);
            return *this;
        }

        ResPtr<T>& operator=(const ResPtr<T>& dest)
        {
            if (this == &dest)
            {
                return *this;
            }
            m_resType = dest.m_resType;
            m_guid = dest.m_guid;
            m_instance  = dest.m_instance;
            return *this;
        }

        ResPtr<T>& operator=(ResPtr<T>&& dest)
        {
            if (this == &dest)
            {
                return *this;
            }
            m_resType = dest.m_resType;
            m_guid = dest.m_guid;
            m_instance  = dest.m_instance;
            return *this;
        }

        StringText GetPath() const { return m_guid; }
        StringText GetGUID() const { return m_guid; }
        StringText& GetGuidReference() { return m_guid; }
        
        EResType GetResType() const { return m_resType; }
        void SetResType(EResType name) { m_resType = name; }

        bool operator==(const T* ptr) const { return (m_instance == ptr); }

        bool operator!=(const T* ptr) const { return (m_instance != ptr); }

        bool operator==(const ResPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }

        bool operator!=(const ResPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

        template<typename T1>
        explicit operator T1*()
        {
            return static_cast<T1*>(m_instance);
        }

        template<typename T1>
        operator ResPtr<T1>()
        {
            return ResPtr<T1>(m_resType, (T1*)(m_instance));
        }

        template<typename T1>
        explicit operator const T1*() const
        {
            return static_cast<T1*>(m_instance);
        }

        template<typename T1>
        operator const ResPtr<T1>() const
        {
            return ResPtr<T1>(m_resType, (T1*)(m_instance));
        }

        T* operator->() { return m_instance; }

        T* operator->() const { return m_instance; }

        T& operator*() { return *(m_instance); }

        T* GetPtr() { return m_instance; }

        T* GetPtr() const { return m_instance; }

        const T& operator*() const { return *(static_cast<const T*>(m_instance)); }

        T*& GetPtrReference() { return m_instance; }

        operator bool() const { return (m_instance != nullptr); }

        bool IsNull (){ return m_instance == nullptr;}
    
    protected:
        EResType    m_resType;
        StringText  m_guid = {""};
        T*          m_instance {nullptr};
    public:
        typedef T   m_type;
    };
    
#define DECLARE_RES_PTR_TYPE(restypename) \
    class restypename##ResPtr :public ResPtr<##restypename##>\
    {\
    public:\
        static restypename##ResPtr NullPtr;\
    public:\
        restypename##ResPtr(StringText guid, ##restypename##* instance) :ResPtr(EResType::ERes##restypename,guid,instance)  {}\
        restypename##ResPtr() :ResPtr(EResType::ERes##restypename,"",nullptr)  {}\
    };\
    template<>\
    Json JsonSerializer::Write(const restypename##ResPtr& instance);\
    template<>\
    restypename##ResPtr& JsonSerializer::Read(const Json& json_context, restypename##ResPtr& instance);\

    class ShaderResPtr : public ResPtr<Shader>
    {
    public:
        static ShaderResPtr NullPtr;

    public:
        ShaderResPtr(std::string guid, Shader* instance) : ResPtr(EResType::EResShader, guid, instance)
        {
        }

        ShaderResPtr() : ResPtr(EResType::EResShader, "", nullptr)
        {
        }
    };

    template <>
    Json JsonSerializer::Write(const ShaderResPtr& instance);
    template <>
    ShaderResPtr& JsonSerializer::Read(const Json& json_context, ShaderResPtr& instance);
    
    //DECLARE_RES_PTR_TYPE(Shader) 
    DECLARE_RES_PTR_TYPE(Texture)
    DECLARE_RES_PTR_TYPE(Model) 
    DECLARE_RES_PTR_TYPE(Animation) 
    DECLARE_RES_PTR_TYPE(Sound) 
    DECLARE_RES_PTR_TYPE(Material)

}
