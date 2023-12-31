/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "algorithm"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/SceneSystem/Scene.h"
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
            // ignore missing component
            if(comp == nullptr)
                continue;
            SharedPtr<Component> sptr;
            sptr.reset(comp.GetPtr());
            m_components.push_back(sptr);
        }
        for (auto child : p_resActor.m_childrenIds)
        {
            m_childrenIds.push_back(child);
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
            p_resActor.m_components.push_back( Reflection::MetaPtr<Component>(comp->GetTypeName(),comp.get()));
        }
        for (auto child : m_childrenIds)
        {
            p_resActor.m_childrenIds.push_back(child);
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
        if(p_parent == nullptr && m_parentID == k_invalidID) return;
        if(p_parent->m_actorID == m_parentID ) return;
        
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
            p_parent->m_childrenIds.push_back(m_actorID);
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
            auto& childrenIds = parent->m_childrenIds;
            for (auto it = childrenIds.begin();it!= childrenIds.end();++it)
            {
                if(*it == m_actorID){
                    childrenIds.erase(it);
                    break;
                }
            }
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
        for (auto child : m_childrenIds)
            m_scene->DestroyActor(child);
        m_scene->DestroyActor(this);
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

    CompVector& Actor::GetComponentsCopy(CompVector& comps)
    {
        m_tempComponents.clear();
        for (auto item : comps)
        {
            m_tempComponents.push_back(item);
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
        {
            NotifyComponentRemoved(component);
            component->OnDestroy();
        }

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

  
    SharedPtr<Component> Actor::GetComponent(TypeID typeId)
    {
        if(!TypeUtil::IsSubclassOf<Component>(typeId))
            return nullptr;
        for (auto& item : m_components)
        {
            if(item->IsAssignableFrom(typeId))
                return item;
        }
        return nullptr;
    }
    SharedPtr<Component> Actor::AddComponent(TypeID typeId)
    {
        if(!TypeUtil::IsSubclassOf<Component>(typeId))
            return nullptr;
        Component* compRawPtr = (Component*)TypeUtil::CreateInstance(typeId);
        SharedPtr<Component> comp;
        comp.reset(compRawPtr);
        OnAddComponent(comp);
        return comp;
    }

    bool Actor::RemoveComponent(SharedPtr<Component> p_component)
    {
        if(p_component == nullptr) return false;
        for (auto it = m_components.begin();it!= m_components.end();++it)
        {
            if (it->get() == p_component.get())
            {
                NotifyComponentRemoved(p_component);
                m_components.erase(it);
                return true;
            }
        }
        return false;
    }

    bool Actor::HasComponent(TypeID typeId)
    {
        return GetComponent(typeId) != nullptr;
    }

    CompVector& Actor::GetComponentsInternal()
    {
        return GetComponentsCopy(m_components);
    }

    void Actor::OnAddComponent(SharedPtr<Component>& comp)
    {
        ActorPtr ptr = GetSceneActor(m_actorID);
        LE_ASSERT(ptr!= nullptr,"Can not find a actor in scene" + std::to_string(m_actorID));
        comp->SetActor(ptr);
        m_components.push_back(comp);
        comp->OnAwake();
        NotifyComponentAdd(comp);
        if (m_playing && IsActive())
        {
            comp->OnEnable();
            comp->OnStart();
        }		
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
