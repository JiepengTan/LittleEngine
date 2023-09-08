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
        ObjectID   m_objectId;
    };
    
    REFLECTION_TYPE(ResActor)
    CLASS(ResActor: public ResData, Fields)
    {
        REFLECTION_BODY(ResActor)
    public:
        StringName      m_prefabGuid;
        
        ObjectID        m_instanceId;
        StringName		m_name = "";
        StringName		m_tag = "";
        bool			m_active = true;
        ActorID	        m_actorID = 0;
        ActorID			m_parentID = 0;
        std::vector<Reflection::ReflectionPtr<Component>> m_components;
    };
    
    REFLECTION_TYPE(ResPrefab)
    CLASS(ResPrefab: public ResObject, Fields)
    {
        REFLECTION_BODY(ResPrefab)
    public:
        std::vector<Component*> components;
    };
    REFLECTION_TYPE(ResScene)
    CLASS(ResScene: public ResObject, Fields)
    {
        REFLECTION_BODY(ResScene)
    public:
        int m_sceneId;
        std::vector<LittleEngine::ResActor> actors;
    };
}
