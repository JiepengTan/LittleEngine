﻿#pragma once
#include "Macro.h"
#include "Core/Reflection/Reflection.h"
#include "tinyxml2/tinyxml2.h"


namespace LittleEngine
{
    
    constexpr ObjectID k_maxID = std::numeric_limits<std::size_t>::max();
    constexpr ObjectID k_invalidID = std::numeric_limits<std::size_t>::max();
    class ObjectIDAllocator
    {
    public:
        static ObjectID Alloc();
        static void Init(ObjectID p_startId);
    private:
        static std::atomic<ObjectID> m_next_id;
    };
    
    class ActorIDAllocator
    {
    public:
        static ActorID Alloc();
        static void Init(ObjectID p_startId);
    private:
        static std::atomic<ActorID> m_next_id;
    };
    namespace Reflection 
    { 
        namespace TypeFieldReflectionOparator 
        { 
            class TypeObjectOperator; 
        } 
    };
    
    REFLECTION_TYPE(Object)
    CLASS(Object, WhiteListFields)
    {
        REFLECTION_BODY(Object)
    public:
        Object() = default;
        virtual ~Object() = default;
        /**
        * Serialize all the components
        */
        virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot);
        /**
        * Deserialize all the components
        */
        virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot);

        ObjectID GetObjectId(){  return m_instanceId;}
    private:
        ObjectID m_instanceId;
    };
}
