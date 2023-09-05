#pragma once
#include "Resource/ResIncludeBasic.h"
#ifdef false
namespace LittleEngine
{
    REFLECTION_TYPE(ResComponent)
    CLASS(ResComponent: public ResData, Fields)
    {
        REFLECTION_BODY(ResComponent)
    public:
    };
    
    REFLECTION_TYPE(ResActor)
    CLASS(ResActor: public ResData, Fields)
    {
        REFLECTION_BODY(ResActor)
    public:
        std::vector<LittleEngine::Reflection::ReflectionPtr<LittleEngine::AComponent>> components;
    };
    
    REFLECTION_TYPE(ResActor)
    CLASS(ResPrefab: public ResObject, Fields)
    {
        REFLECTION_BODY(ResActor)
        public:
        std::vector<LittleEngine::Reflection::ReflectionPtr<LittleEngine::AComponent>> components;
    };
    REFLECTION_TYPE(ResActor)
    CLASS(ResScene: public ResObject, Fields)
    {
        REFLECTION_BODY(ResActor)
        public:
        std::vector<LittleEngine::Reflection::ReflectionPtr<LittleEngine::AComponent>> components;
    };
}

#endif