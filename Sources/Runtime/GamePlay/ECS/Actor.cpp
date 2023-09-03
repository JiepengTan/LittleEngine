/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "GamePlay/ECS/Actor.h"

#include "GamePlay/ECS/Components/CPhysicalBox.h"
#include "GamePlay/ECS/Components/CPhysicalSphere.h"
#include "GamePlay/ECS/Components/CPhysicalCapsule.h"
#include "GamePlay/ECS/Components/CCamera.h"
#include "GamePlay/ECS/Components/CModelRenderer.h"
#include "GamePlay/ECS/Components/CMaterialRenderer.h"
#include "GamePlay/ECS/Components/CAudioSource.h"
#include "GamePlay/ECS/Components/CAudioListener.h"
#include "GamePlay/ECS/Components/CPointLight.h"
#include "GamePlay/ECS/Components/CDirectionalLight.h"
#include "GamePlay/ECS/Components/CSpotLight.h"
#include "GamePlay/ECS/Components/CAmbientBoxLight.h"
#include "GamePlay/ECS/Components/CAmbientSphereLight.h"
#include "GamePlay/ECS/Components/CAnimator.h"
#include "GamePlay/SceneSystem/Scene.h"

OvTools::Eventing::Event<OvCore::ECS::Actor&> OvCore::ECS::Actor::DestroyedEvent;
OvTools::Eventing::Event<OvCore::ECS::Actor&> OvCore::ECS::Actor::CreatedEvent;
OvTools::Eventing::Event<OvCore::ECS::Actor&, OvCore::ECS::Actor&> OvCore::ECS::Actor::AttachEvent;
OvTools::Eventing::Event<OvCore::ECS::Actor&> OvCore::ECS::Actor::DettachEvent;

OvCore::ECS::Actor::Actor(SceneSystem::Scene* p_scene, int64_t p_actorID, const std::string & p_name, const std::string & p_tag, bool& p_playing) :
	Owner(p_scene),
	m_aliveState(EActorAliveState::Alive),
	m_name(p_name),
	m_tag(p_tag),
	m_playing(p_playing),
	m_actorID(p_actorID),
	transform(AddComponent<Components::CTransform>())
{
	OVLOG( "scene " + std::to_string(p_scene->GetSceneId()) +  " Create Actor " + std::to_string(p_actorID) + "  name =" + p_name);
	CreatedEvent.Invoke(*this);
}

OvCore::ECS::Actor::~Actor()
{
	
}

const std::string & OvCore::ECS::Actor::GetName() const
{
	return m_name;
}

const std::string & OvCore::ECS::Actor::GetTag() const
{
	return m_tag;
}

void OvCore::ECS::Actor::SetName(const std::string & p_name)
{
	m_name = p_name;
}

void OvCore::ECS::Actor::SetTag(const std::string & p_tag)
{
	m_tag = p_tag;
}

void OvCore::ECS::Actor::SetActive(bool p_active)
{
	if (p_active != m_active)
	{
		RecursiveWasActiveUpdate();
		m_active = p_active;
		RecursiveActiveUpdate();
	}
}

bool OvCore::ECS::Actor::IsSelfActive() const
{
	return m_active;
}

bool OvCore::ECS::Actor::IsActive() const
{
	return m_active && (m_parent ? m_parent->IsActive() : true);
}

void OvCore::ECS::Actor::SetID(int64_t p_id)
{
	m_actorID = p_id;
}

int64_t OvCore::ECS::Actor::GetID() const
{
	return m_actorID;
}

void OvCore::ECS::Actor::SetParent(Actor& p_parent)
{
	DetachFromParent();

	/* Define the given parent as the new parent */
	m_parent = &p_parent;
	m_parentID = p_parent.m_actorID;
	transform.SetParent(p_parent.transform);

	/* Store the actor in the parent children list */
	p_parent.m_children.push_back(this);

	AttachEvent.Invoke(*this, p_parent);
}

void OvCore::ECS::Actor::DetachFromParent()
{
	DettachEvent.Invoke(*this);

	/* Remove the actor from the parent children list */
	if (m_parent && m_parent->IsAlive())
	{
		m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](Actor* p_element)
		{
			return p_element == this;
		}));
	}

	m_parent = nullptr;
	m_parentID = 0;

	transform.RemoveParent();
}

bool OvCore::ECS::Actor::HasParent() const
{
	return m_parent;
}

OvCore::ECS::Actor * OvCore::ECS::Actor::GetParent() const
{
	return m_parent;
}

int64_t OvCore::ECS::Actor::GetParentID() const
{
	return m_parentID;
}

std::vector<OvCore::ECS::Actor*>& OvCore::ECS::Actor::GetChildren()
{
	return m_children;
}
bool OvCore::ECS::Actor::Destroy()
{
	if(!IsAlive()) return false;
	Owner->DestroyActor(*this);
	for (auto child : m_children)
		child->Destroy();
	return true;
}

void OvCore::ECS::Actor::MarkAsDestroying()
{
	m_aliveState = EActorAliveState::Destroying;
}
void OvCore::ECS::Actor::MarkAsDestroyed()
{
	m_aliveState = EActorAliveState::Destroyed;
}
bool OvCore::ECS::Actor::IsAlive() const
{
	return m_aliveState == EActorAliveState::Alive ;
}
bool OvCore::ECS::Actor::IsDestroying()const
{
	return m_aliveState == EActorAliveState::Destroying ;
}
bool OvCore::ECS::Actor::IsDestroyed()const
{
	return m_aliveState == EActorAliveState::Destroyed ;
}
void OvCore::ECS::Actor::SetSleeping(bool p_sleeping)
{
	m_sleeping = p_sleeping;
}

std::vector<std::shared_ptr<OvCore::ECS::Components::AComponent>>& OvCore::ECS::Actor::GetComponentsCopy(
	std::vector<std::shared_ptr<OvCore::ECS::Components::AComponent>>& comps)
{
	m_tempComponents.clear();
	for (auto ident : comps){
		m_tempComponents.push_back(ident);
	}
	return m_tempComponents;
}

void OvCore::ECS::Actor::OnAwake()
{
	if(m_awaked) return;
	m_awaked = true;
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnAwake(); });
}

void OvCore::ECS::Actor::OnStart()
{
	if(m_started) return;
	m_started = true;
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnStart(); });
}

void OvCore::ECS::Actor::OnEnable()
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnEnable(); });
}

void OvCore::ECS::Actor::OnDisable()
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnDisable(); });
}

void OvCore::ECS::Actor::OnDestroy()
{
	if (!m_sleeping)
	{
		if (IsActive())
			OnDisable();
	}
	DestroyedEvent.Invoke(*this);
	
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnDestroy(); });
	std::for_each(comps.begin(), comps.end(), [&](std::shared_ptr<Components::AComponent> p_component) { ComponentRemovedEvent.Invoke(*p_component); });

	DetachFromParent();
	
	MarkAsDestroyed();
	m_components.clear();
	m_tempComponents.clear();
	m_children.clear();
}

void OvCore::ECS::Actor::OnUpdate(float p_deltaTime)
{
	if (IsAlive()&& IsActive())
	{
		auto comps = GetComponentsCopy(m_components);
		std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnUpdate(p_deltaTime); });
	}
}

void OvCore::ECS::Actor::OnFixedUpdate(float p_deltaTime)
{
	if (IsAlive()&&IsActive())
	{
		auto comps = GetComponentsCopy(m_components);
		std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnFixedUpdate(p_deltaTime); });
	}
}

void OvCore::ECS::Actor::OnLateUpdate(float p_deltaTime)
{
	if (IsAlive()&&IsActive())
	{
		auto comps = GetComponentsCopy(m_components);
		std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnLateUpdate(p_deltaTime); });
	}
}

void OvCore::ECS::Actor::OnCollisionEnter(Components::CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionEnter(p_otherObject); });
}

void OvCore::ECS::Actor::OnCollisionStay(Components::CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionStay(p_otherObject); });
}

void OvCore::ECS::Actor::OnCollisionExit(Components::CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionExit(p_otherObject); });
}

void OvCore::ECS::Actor::OnTriggerEnter(Components::CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerEnter(p_otherObject); });
}

void OvCore::ECS::Actor::OnTriggerStay(Components::CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerStay(p_otherObject); });
}

void OvCore::ECS::Actor::OnTriggerExit(Components::CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerExit(p_otherObject); });
}

bool OvCore::ECS::Actor::RemoveComponent(OvCore::ECS::Components::AComponent& p_component)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (it->get() == &p_component)
		{
			ComponentRemovedEvent.Invoke(p_component);
			m_components.erase(it);
			return true;
		}
	}
	return false;
}

std::vector<std::shared_ptr<OvCore::ECS::Components::AComponent>>& OvCore::ECS::Actor::GetComponents()
{
	return m_components;
}


void OvCore::ECS::Actor::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_actorsRoot)
{
	tinyxml2::XMLNode* actorNode = p_doc.NewElement("actor");
	p_actorsRoot->InsertEndChild(actorNode);

	OvCore::Helpers::Serializer::SerializeString(p_doc, actorNode, "name", m_name);
	OvCore::Helpers::Serializer::SerializeString(p_doc, actorNode, "tag", m_tag);
	OvCore::Helpers::Serializer::SerializeBoolean(p_doc, actorNode, "active", m_active);
	OvCore::Helpers::Serializer::SerializeInt64(p_doc, actorNode, "id", m_actorID);
	OvCore::Helpers::Serializer::SerializeInt64(p_doc, actorNode, "parent", m_parentID);

	tinyxml2::XMLNode* componentsNode = p_doc.NewElement("components");
	actorNode->InsertEndChild(componentsNode);

	for (auto& component : m_components)
	{
		/* Current component root */
		tinyxml2::XMLNode* componentNode = p_doc.NewElement("component");
		componentsNode->InsertEndChild(componentNode);

		/* Component type */
		OvCore::Helpers::Serializer::SerializeString(p_doc, componentNode, "type", typeid(*component).name());

		/* Data node (Will be passed to the component) */
		tinyxml2::XMLElement* data = p_doc.NewElement("data");
		componentNode->InsertEndChild(data);

		/* Data serialization of the component */
		component->OnSerialize(p_doc, data);
	}

	tinyxml2::XMLNode* behavioursNode = p_doc.NewElement("behaviours");
	actorNode->InsertEndChild(behavioursNode);
}
void OvCore::ECS::Actor::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_actorsRoot)
{
	OvCore::Helpers::Serializer::DeserializeString(p_doc, p_actorsRoot, "name", m_name);
	OvCore::Helpers::Serializer::DeserializeString(p_doc, p_actorsRoot, "tag", m_tag);
	OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_actorsRoot, "active", m_active);
	OvCore::Helpers::Serializer::DeserializeInt64(p_doc, p_actorsRoot, "id", m_actorID);
	OvCore::Helpers::Serializer::DeserializeInt64(p_doc, p_actorsRoot, "parent", m_parentID);

	{
		tinyxml2::XMLNode* componentsRoot = p_actorsRoot->FirstChildElement("components");
		if (componentsRoot)
		{
			tinyxml2::XMLElement* currentComponent = componentsRoot->FirstChildElement("component");

			while (currentComponent)
			{
				std::string componentType = currentComponent->FirstChildElement("type")->GetText();
				OvCore::ECS::Components::AComponent* component = nullptr;

				// TODO: Use component name instead of typeid (unsafe)
				if (componentType == typeid(Components::CTransform).name())			component = &transform;
				else if (componentType == typeid(Components::CPhysicalBox).name())			component = &AddComponent<OvCore::ECS::Components::CPhysicalBox>();
				else if (componentType == typeid(Components::CPhysicalSphere).name())		component = &AddComponent<OvCore::ECS::Components::CPhysicalSphere>();
				else if (componentType == typeid(Components::CPhysicalCapsule).name())		component = &AddComponent<OvCore::ECS::Components::CPhysicalCapsule>();
				else if (componentType == typeid(Components::CModelRenderer).name())			component = &AddComponent<OvCore::ECS::Components::CModelRenderer>();
				else if (componentType == typeid(Components::CCamera).name())				component = &AddComponent<OvCore::ECS::Components::CCamera>();
				else if (componentType == typeid(Components::CMaterialRenderer).name())		component = &AddComponent<OvCore::ECS::Components::CMaterialRenderer>();
				else if (componentType == typeid(Components::CAudioSource).name())			component = &AddComponent<OvCore::ECS::Components::CAudioSource>();
				else if (componentType == typeid(Components::CAudioListener).name())		component = &AddComponent<OvCore::ECS::Components::CAudioListener>();
				else if (componentType == typeid(Components::CPointLight).name())			component = &AddComponent<OvCore::ECS::Components::CPointLight>();
				else if (componentType == typeid(Components::CDirectionalLight).name())		component = &AddComponent<OvCore::ECS::Components::CDirectionalLight>();
				else if (componentType == typeid(Components::CSpotLight).name())			component = &AddComponent<OvCore::ECS::Components::CSpotLight>();
				else if (componentType == typeid(Components::CAmbientBoxLight).name())		component = &AddComponent<OvCore::ECS::Components::CAmbientBoxLight>();
				else if (componentType == typeid(Components::CAmbientSphereLight).name())	component = &AddComponent<OvCore::ECS::Components::CAmbientSphereLight>();
				else if (componentType == typeid(Components::CAnimator).name())				component = &AddComponent<OvCore::ECS::Components::CAnimator>();

				if (component)
					component->OnDeserialize(p_doc, currentComponent->FirstChildElement("data"));

				currentComponent = currentComponent->NextSiblingElement("component");
			}
		}
	}
}

void OvCore::ECS::Actor::RecursiveActiveUpdate()
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

	for (auto child : m_children)
		child->RecursiveActiveUpdate();
}

void OvCore::ECS::Actor::RecursiveWasActiveUpdate()
{
	m_wasActive = IsActive();
	for (auto child : m_children)
		child->RecursiveWasActiveUpdate();
}
