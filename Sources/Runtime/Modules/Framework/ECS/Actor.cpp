/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "Modules/Framework/ECS/Actor.h"

#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CAudioSource.h"
#include "Modules/Framework/ECS/Components/CAudioListener.h"
#include "Modules/Framework/ECS/Components/CPointLight.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CSpotLight.h"
#include "Modules/Framework/ECS/Components/CAmbientBoxLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"
#include "Modules/Framework/ECS/Components/CAnimator.h"
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
        }else
        {
            m_parentID = p_parent->m_actorID;
            transform->SetParent(p_parent->transform);
            p_parent->m_childrenIds.emplace(m_actorID);
            Scene::AttachEvent.Invoke(m_scene->GetActor(m_actorID), p_parent);
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

    void Actor::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
    {
        tinyxml2::XMLNode* actorNode = p_doc.NewElement("actor");
        p_actorsRoot->InsertEndChild(actorNode);

        Serializer::SerializeString(p_doc, actorNode, "name", m_name);
        Serializer::SerializeString(p_doc, actorNode, "tag", m_tag);
        Serializer::SerializeBoolean(p_doc, actorNode, "active", m_active);
        Serializer::SerializeInt64(p_doc, actorNode, "id", m_actorID);
        Serializer::SerializeInt64(p_doc, actorNode, "parent", m_parentID);

        tinyxml2::XMLNode* componentsNode = p_doc.NewElement("components");
        actorNode->InsertEndChild(componentsNode);

        for (auto& item : m_components)
        {
            auto component = item.second;
            /* Current component root */
            tinyxml2::XMLNode* componentNode = p_doc.NewElement("component");
            componentsNode->InsertEndChild(componentNode);

            /* Component type */
            Serializer::SerializeString(p_doc, componentNode, "type", typeid(*component).name());

            /* Data node (Will be passed to the component) */
            tinyxml2::XMLElement* data = p_doc.NewElement("data");
            componentNode->InsertEndChild(data);

            /* Data serialization of the component */
            component->OnSerialize(p_doc, data);
        }

        tinyxml2::XMLNode* behavioursNode = p_doc.NewElement("behaviours");
        actorNode->InsertEndChild(behavioursNode);
    }

    void Actor::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
    {
        Serializer::DeserializeString(p_doc, p_actorsRoot, "name", m_name);
        Serializer::DeserializeString(p_doc, p_actorsRoot, "tag", m_tag);
        Serializer::DeserializeBoolean(p_doc, p_actorsRoot, "active", m_active);
        Serializer::DeserializeUInt64(p_doc, p_actorsRoot, "id", m_actorID);
        Serializer::DeserializeUInt64(p_doc, p_actorsRoot, "parent", m_parentID);

        {
            tinyxml2::XMLNode* componentsRoot = p_actorsRoot->FirstChildElement("components");
            if (componentsRoot)
            {
                tinyxml2::XMLElement* currentComponent = componentsRoot->FirstChildElement("component");

                while (currentComponent)
                {
                    std::string componentType = currentComponent->FirstChildElement("type")->GetText();
                    SharedPtr<Component> component = nullptr;

                    // TODO: Use component name instead of typeid (unsafe)
                    if (componentType == typeid(CTransform).name()) component = transform;
                    else if (componentType == typeid(CPhysicalBox).name()) component = AddComponent<
                        CPhysicalBox>();
                    else if (componentType == typeid(CPhysicalSphere).name()) component = AddComponent<
                        CPhysicalSphere>();
                    else if (componentType == typeid(CPhysicalCapsule).name()) component = AddComponent<
                        CPhysicalCapsule>();
                    else if (componentType == typeid(CModelRenderer).name()) component = AddComponent<
                        CModelRenderer>();
                    else if (componentType == typeid(CCamera).name()) component = AddComponent<
                        CCamera>();
                    else if (componentType == typeid(CMaterialRenderer).name()) component = AddComponent<
                        CMaterialRenderer>();
                    else if (componentType == typeid(CAudioSource).name()) component = AddComponent<
                        CAudioSource>();
                    else if (componentType == typeid(CAudioListener).name()) component = AddComponent<
                        CAudioListener>();
                    else if (componentType == typeid(CPointLight).name()) component = AddComponent<
                        CPointLight>();
                    else if (componentType == typeid(CDirectionalLight).name()) component = AddComponent<
                        CDirectionalLight>();
                    else if (componentType == typeid(CSpotLight).name()) component = AddComponent<
                        CSpotLight>();
                    else if (componentType == typeid(CAmbientBoxLight).name()) component = AddComponent<
                        CAmbientBoxLight>();
                    else if (componentType == typeid(CAmbientSphereLight).name()) component = AddComponent<
                        CAmbientSphereLight>();
                    else if (componentType == typeid(CAnimator).name()) component = AddComponent<
                        CAnimator>();

                    if (component)
                        component->OnDeserialize(p_doc, currentComponent->FirstChildElement("data"));

                    currentComponent = currentComponent->NextSiblingElement("component");
                }
            }
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
}
