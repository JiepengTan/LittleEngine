#pragma once
#include "Resource/ResIncludeBasic.h"

namespace LittleEngine
{
    class Component;
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
        StringName prefabGuid;
        std::vector<Reflection::ReflectionPtr<Component>> components;
    };
    
    REFLECTION_TYPE(ResPrefab)
    CLASS(ResPrefab: public ResObject, Fields)
    {
        REFLECTION_BODY(ResPrefab)
        public:
        std::vector<Reflection::ReflectionPtr<Component>> components;
    };
    REFLECTION_TYPE(ResScene)
    CLASS(ResScene: public ResObject, Fields)
    {
        REFLECTION_BODY(ResScene)
    public:
        
        META(Enable)
        int Levels;
        std::vector<Reflection::ReflectionPtr<Component>> actors;
    };
}
