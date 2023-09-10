#pragma once
#include "ResIncludeBasic.h"
#include "Resource/Asset/UniformType.h"
#include "Resource/Core/ResPtr.h"

namespace LittleEngine
{

    class Component;
    REFLECTION_TYPE(ResLight)
    STRUCT(ResLight: public ResData, Fields)
    {
        REFLECTION_BODY(ResLight)
    public:
        FVector3	        color		= { 1.f, 1.f, 1.f };
        float				intensity	= 1.f;
        float				constant	= 0.0f;
        float				linear		= 0.0f;
        float				quadratic	= 1.0f;
        float				cutoff		= 12.f;
        float				outerCutoff = 15.f;
        float				type		= 0.0f;
    };
    REFLECTION_TYPE(ResUniformInfo)
    STRUCT(ResUniformInfo: public ResData, Fields)
    {
        REFLECTION_BODY(ResUniformInfo)
    public:
        FVector4    m_data = FVector4::Zero;
        
        UniformType    m_uniformType = UniformType::UNIFORM_SAMPLER_CUBE;
        std::string    m_key = "";
        TextureResPtr   m_texture = TextureResPtr::NullPtr;

        bool GetBool() const { return m_data.x>0.5f;}
        int GetInt() const{ return (int)(m_data.x+0.5f);}
        float GetFloat() const{ return m_data.x;}
        FVector2 GetVector2()const { return FVector2(m_data.x,m_data.y);}
        FVector3 GetVector3()const { return FVector3(m_data.x,m_data.y,m_data.z);}
        FVector4 GetVector4()const { return m_data;}
        
        void Get(bool& val) const { val=GetBool();}
        void Get(int& val) const{ val= GetInt();}
        void Get(float& val) const{ val= GetFloat();}
        void Get(FVector2& val)const { val= GetVector2();}
        void Get(FVector3& val)const { val= GetVector3();}
        void Get(FVector4& val)const { val= GetVector4();}
        
        void Set(const bool& val){ m_data.x = val?1.0f:0.0f; }
        void Set(const int& val){ m_data.x = (float)val;}
        void Set(const float& val){ m_data.x = val;}
        void Set(const FVector2& val){ m_data.x = val.x;m_data.y = val.y;}
        void Set(const FVector3& val){ m_data.x = val.x;m_data.y = val.y;m_data.z = val.z;}
        void Set(const FVector4& val){ m_data = val;}
        void Set(Texture* texture){m_texture = TextureResPtr::NullPtr;}
        
        void SetBool(const bool& val){ Set(val); }
        void SetInt(const int& val){  Set(val);}
        void SetFloat(const float& val){  Set(val);}
        void SetVector2(const FVector2& val){  Set(val);}
        void SetVector3(const FVector3& val){  Set(val);}
        void SetVector4(const FVector4& val){  Set(val);}
        void SetTexture(Texture* val){ Set(val);}
    };
    
    REFLECTION_TYPE(ResMaterial)
    STRUCT(ResMaterial: public ResData, Fields)
    {
        REFLECTION_BODY(ResMaterial)
    public:
        bool m_blendable		= false;
        bool m_backfaceCulling	= true;
        bool m_frontfaceCulling = false;
        bool m_depthTest		= true;
        bool m_depthWriting		= true;
        bool m_colorWriting		= true;
        int  m_gpuInstances		= 1;
		ShaderResPtr        m_shader = ShaderResPtr::NullPtr;
		// TODO tanjp use map instead of vector
        std::vector<ResUniformInfo> m_uniformsData = {};
    };
    
}
