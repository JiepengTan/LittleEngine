#pragma once
#include "ResIncludeBasic.h"
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
}