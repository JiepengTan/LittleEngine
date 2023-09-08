#pragma once
#include "ResIncludeBasic.h"
#include "Modules/Rendering/Resources/UniformType.h"

namespace LittleEngine
{

    class Component;
    REFLECTION_TYPE(ResLight)
    CLASS(ResLight: public ResData, Fields)
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
        std::string    key;
        UniformType     m_uniformType;
        bool m_bool		    ;
        int  m_int	        ;
        float m_float	    ;
        FVector4 m_vec4	    ;
        FMatrix4 m_matrix4	;
        std::string    texturePath;
    };
    
    REFLECTION_TYPE(ResMaterial)
    CLASS(ResMaterial: public ResData, Fields)
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
		LittleEngine::Rendering::Resources::Shader* m_shader = nullptr;
        std::vector<ResUniformInfo> m_uniformsData;
    };
    
}
