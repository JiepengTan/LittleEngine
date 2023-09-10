/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "algorithm"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/SceneSystem/Scene.h"

namespace LittleEngine
{
    
    Actor::Actor()= default;

    void Actor::DoInit(Scene* p_scene, ActorID p_actorID, const std::string& p_name,
                       const std::string& p_tag, bool& p_playing) 
    {
        m_actorID = p_actorID;
        m_scene = p_scene;
        m_name = p_name;
        m_tag = p_tag;
        m_playing = p_playing;
        transform = AddComponent<CTransform>();
    }
    Actor::~Actor()
    {
    }
    
    void Actor::OnBeforeSceneSave(Scene* p_scene)
    {
        
    }
    void Actor::OnAfterSceneLoaded(Scene* p_scene)
    {
        auto comps = GetComponentsCopy(m_components);
        auto self = m_scene->GetActor(m_actorID);
        for (auto comp : comps)
        {
            comp->owner = self;
            comp->OnAfterSceneLoaded(comp->owner);
        }
    }
    void Actor::LoadFrom(ResActor& p_resActor)
    {
        m_prefabGuid = p_resActor.m_prefabGuid;
        m_instanceId = p_resActor.m_instanceId;
        m_name = p_resActor.m_name;
        m_tag = p_resActor.m_tag;
        m_active = p_resActor.m_active;
        m_actorID = p_resActor.m_actorID;
        m_parentID = p_resActor.m_parentID;
        for (auto comp : p_resActor.m_components)
        {
            SharedPtr<Component> sptr;
            sptr.reset(comp.GetPtr());
            m_components[comp->GetTypeID()] = sptr;
        }
        for (auto child : p_resActor.m_childrenIds)
        {
            m_childrenIds.emplace(child);
        }
        transform = GetComponent<CTransform>();
    }
    void Actor::SaveTo(ResActor& p_resActor)
    {
        p_resActor.m_prefabGuid = m_prefabGuid;
        p_resActor.m_instanceId = m_instanceId;
        p_resActor.m_name = m_name;
        p_resActor.m_tag = m_tag;
        p_resActor.m_active = m_active;
        p_resActor.m_actorID = m_actorID;
        p_resActor.m_parentID = m_parentID;
        auto& comps = GetComponentsCopy(m_components);
        for (auto comp : comps)
        {
            p_resActor.m_components.push_back( Reflection::ReflectionPtr<Component>(comp->GetTypeName(),comp.get()));
        }
    }

    const std::string& Actor::GetName() const
    {
        return m_name;
    }

    const std::string& Actor::GetTag() const
    {
        return m_tag;
    }

    void Actor::SetName(const std::string& p_name)
    {
        m_name = p_name;
    }

    void Actor::SetTag(const std::string& p_tag)
    {
        m_tag = p_tag;
    }

    void Actor::SetActive(bool p_active)
    {
        if (p_active != m_active)
        {
            RecursiveWasActiveUpdate();
            m_active = p_active;
            RecursiveActiveUpdate();
        }
    }

    bool Actor::IsSelfActive() const
    {
        return m_active;
    }

    bool Actor::IsActive() const
    {
        if(!m_active) return false;
        auto parent = GetParent();
        return (parent ? parent->IsActive() : true);
    }

    void Actor::SetID(ActorID p_id)
    {
        m_actorID = p_id;
    }

    ActorID Actor::GetID() const
    {
        return m_actorID;
    }

    void Actor::SetParent(ActorPtr p_parent)
    {
        DetachFromParent();
        if(p_parent == nullptr)
        {
            m_parentID = k_invalidID;
            transform->SetParent(nullptr);
            m_scene->OnActorDetach(m_actorID,m_parentID);
        }else
        {
            m_parentID = p_parent->m_actorID;
            transform->SetParent(p_parent->transform);
            p_parent->m_childrenIds.emplace(m_actorID);
            m_scene->OnActorAttach(m_actorID,m_parentID);
        }
        /* Define the given parent as the new parent */

        /* Store the actor in the parent children list */
    }

    void Actor::DetachFromParent()
    {

        /* Remove the actor from the parent children list */
        auto parent = m_scene->GetActor(m_parentID);
        if (parent && parent->IsAlive())
        {
            parent->m_childrenIds.erase(m_actorID);
        }
        m_scene->OnActorDetach(m_actorID,m_parentID);
        m_parentID = 0;
        transform->RemoveParent();
    }

    bool Actor::HasParent() const
    {
        return m_scene->HasActor(m_parentID);
    }

    ActorPtr Actor::GetParent() const
    {
        if(!m_scene->HasActor(m_parentID)) return nullptr;
        return m_scene->GetActor(m_parentID);
    }

    ActorID Actor::GetParentID() const
    {
        return m_parentID;
    }

    ActorVector Actor::GetChildren() const
    {
        ActorVector result;
        for (auto actorID : m_childrenIds)
        {
            auto actor = m_scene->GetActor(actorID);
            result.push_back(actor);
        }
        return result;
    }

    bool Actor::Destroy()
    {
        if (!IsAlive()) return false;
        m_scene->DestroyActor(this);
        for (auto child : m_childrenIds)
            m_scene->DestroyActor(child);
        return true;
    }

    void Actor::MarkAsDestroying()
    {
        m_aliveState = EActorAliveState::Destroying;
    }

    void Actor::MarkAsDestroyed()
    {
        m_aliveState = EActorAliveState::Destroyed;
    }

    bool Actor::IsAlive() const
    {
        return m_aliveState == EActorAliveState::Alive;
    }

    bool Actor::IsDestroying() const
    {
        return m_aliveState == EActorAliveState::Destroying;
    }

    bool Actor::IsDestroyed() const
    {
        return m_aliveState == EActorAliveState::Destroyed;
    }

    void Actor::SetSleeping(bool p_sleeping)
    {
        m_sleeping = p_sleeping;
    }

    std::vector<std::shared_ptr<Component>>& Actor::GetComponentsCopy(CompMap& comps)
    {
        m_tempComponents.clear();
        for (auto ident : comps)
        {
            m_tempComponents.push_back(ident.second);
        }
        return m_tempComponents;
    }

    void Actor::OnAwake()
    {
        if (m_awaked) return;
        m_awaked = true;
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnAwake(); });
    }

    void Actor::OnStart()
    {
        if (m_started) return;
        m_started = true;
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnStart(); });
    }

    void Actor::OnEnable()
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnEnable(); });
    }

    void Actor::OnDisable()
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnDisable(); });
    }

    void Actor::OnDestroy()
    {
        if (!m_sleeping)
        {
            if (IsActive())
                OnDisable();
        }

        auto comps = GetComponentsCopy(m_components);
        for (auto component : comps)
            component->OnDestroy(); 

        DetachFromParent();

        MarkAsDestroyed();
        m_components.clear();
        m_tempComponents.clear();
        m_childrenIds.clear();
    }

    void Actor::OnUpdate(float p_deltaTime)
    {
        if (IsAlive() && IsActive())
        {
            auto comps = GetComponentsCopy(m_components);
            std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnUpdate(p_deltaTime); });
        }
    }

    void Actor::OnFixedUpdate(float p_deltaTime)
    {
        if (IsAlive() && IsActive())
        {
            auto comps = GetComponentsCopy(m_components);
            std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnFixedUpdate(p_deltaTime); });
        }
    }

    void Actor::OnLateUpdate(float p_deltaTime)
    {
        if (IsAlive() && IsActive())
        {
            auto comps = GetComponentsCopy(m_components);
            std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnLateUpdate(p_deltaTime); });
        }
    }

    void Actor::OnCollisionEnter(SharedPtr<CPhysicalObject> p_otherObject)
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionEnter(p_otherObject); });
    }

    void Actor::OnCollisionStay(SharedPtr<CPhysicalObject> p_otherObject)
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionStay(p_otherObject); });
    }

    void Actor::OnCollisionExit(SharedPtr<CPhysicalObject> p_otherObject)
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionExit(p_otherObject); });
    }

    void Actor::OnTriggerEnter(SharedPtr<CPhysicalObject> p_otherObject)
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerEnter(p_otherObject); });
    }

    void Actor::OnTriggerStay(SharedPtr<CPhysicalObject> p_otherObject)
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerStay(p_otherObject); });
    }

    void Actor::OnTriggerExit(SharedPtr<CPhysicalObject> p_otherObject)
    {
        auto comps = GetComponentsCopy(m_components);
        std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerExit(p_otherObject); });
    }

    bool Actor::RemoveComponent(SharedPtr<Component> p_component)
    {
        if(p_component == nullptr) return false;
        if (m_components.count(p_component->GetTypeID()) != 0)
        {
            m_scene->OnComponentRemoved(p_component);
            m_components.erase(p_component->GetTypeID());
            return true;
        }
        return false;
    }

    CompVector& Actor::GetComponentsInternal()
    {
        return GetComponentsCopy(m_components);
    }

    void Actor::OnSerialize(ISerializer p_serializer)
    {
        SerializeUtil::SerializeString( "name", m_name);
        SerializeUtil::SerializeString( "tag", m_tag);
        SerializeUtil::SerializeBoolean( "active", m_active);
        SerializeUtil::SerializeInt64( "id", m_actorID);
        SerializeUtil::SerializeInt64( "parent", m_parentID);

        SerializeUtil::SerializeInt( "m_components", m_components.size());
        for (auto& item : m_components)
        {
            auto comp = item.second;
            SerializeUtil::SerializeString( "type", comp->GetName());
            auto component = item.second;
            /* Data serialization of the component */
            component->OnSerialize(p_serializer);
        }
    }

    void Actor::OnDeserialize(ISerializer p_serializer)
    {
        SerializeUtil::DeserializeString("name", m_name);
        SerializeUtil::DeserializeString("tag", m_tag);
        SerializeUtil::DeserializeBoolean("active", m_active);
        SerializeUtil::DeserializeUInt64("id", m_actorID);
        SerializeUtil::DeserializeUInt64("parent", m_parentID);

        // TODO
        int count = 0;
        SerializeUtil::DeserializeInt("parent", count);
    }

    void Actor::RecursiveActiveUpdate()
    {
        bool isActive = IsActive();

        if (!m_sleeping)
        {
            if (!m_wasActive && isActive)
            {
                if (!m_awaked)
                    OnAwake();

                OnEnable();

                if (!m_started)
                    OnStart();
            }

            if (m_wasActive && !isActive)
                OnDisable();
        }
        
        for (auto child : m_childrenIds)
        {
            auto actor = m_scene->GetActor(child);
            if(actor!= nullptr)
                actor->RecursiveActiveUpdate();
        }
    }

    void Actor::RecursiveWasActiveUpdate()
    {
        m_wasActive = IsActive();
        for (auto child : m_childrenIds)
        {
            auto actor = m_scene->GetActor(child);
            if(actor!= nullptr)
                actor->RecursiveWasActiveUpdate();
        }
    }
    void Actor::NotifyComponentAdd(CompPtr p_comp)const
    {
        m_scene->OnComponentAdded(p_comp);
    }
    void Actor::NotifyComponentRemoved(CompPtr p_comp) const
    {
        m_scene->OnComponentRemoved(p_comp);
    }
    ActorPtr Actor::GetSceneActor(ActorID p_actorID)
    {
        return m_scene->GetActor(p_actorID);
    }
}
