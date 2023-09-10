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


        void SetBool(bool val){ m_data.x = val?1.0f:0.0f; }
        void SetInt(int val){ m_data.x = (float)val;}
        void SetFloat(float val){ m_data.x = val;}
        void SetVector2(FVector2 val){ m_data.x = val.x;m_data.y = val.y;}
        void SetVector3(FVector3 val){ m_data.x = val.x;m_data.y = val.y;m_data.z = val.z;}
        void SetVector4(FVector4 val){ m_data = val;}
        void SetTexture(Texture* texture){m_texture = TextureResPtr::NullPtr;}
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
        std::vector<ResUniformInfo> m_uniformsData = {};
    };
    
}
