#pragma once
#include "Resource/ResIncludeBasic.h"

namespace OvCore::ResDefines
{
    REFLECTION_TYPE(ResComponent)
    CLASS(ResComponent: public ResStruct, Fields)
    {
        REFLECTION_BODY(ResComponent);
    public:
    };
    
    REFLECTION_TYPE(ResActor)
    CLASS(ResActor: public ResStruct, Fields)
    {
        REFLECTION_BODY(ResActor);
    public:
        std::vector<OvCore::Reflection::ReflectionPtr<OvCore::ECS::Components::AComponent>> components;
    };
    
    REFLECTION_TYPE(ResActor)
    CLASS(ResPrefab: public ResObject, Fields)
    {
        REFLECTION_BODY(ResActor);
        public:
        std::vector<OvCore::Reflection::ReflectionPtr<OvCore::ECS::Components::AComponent>> components;
    };
    REFLECTION_TYPE(ResActor)
    CLASS(ResScene: public ResObject, Fields)
    {
        REFLECTION_BODY(ResActor);
        public:
        std::vector<OvCore::Reflection::ReflectionPtr<OvCore::ECS::Components::AComponent>> components;
    };
}

