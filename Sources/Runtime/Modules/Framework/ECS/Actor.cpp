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

LittleEngine::Eventing::Event<LittleEngine::Actor&> LittleEngine::Actor::DestroyedEvent;
LittleEngine::Eventing::Event<LittleEngine::Actor&> LittleEngine::Actor::CreatedEvent;
LittleEngine::Eventing::Event<LittleEngine::Actor&, LittleEngine::Actor&> LittleEngine::Actor::AttachEvent;
LittleEngine::Eventing::Event<LittleEngine::Actor&> LittleEngine::Actor::DettachEvent;

LittleEngine::Actor::Actor(SceneSystem::Scene* p_scene, int64_t p_actorID, const std::string & p_name, const std::string & p_tag, bool& p_playing) :
	Owner(p_scene),
	m_aliveState(EActorAliveState::Alive),
	m_name(p_name),
	m_tag(p_tag),
	m_playing(p_playing),
	m_actorID(p_actorID),
	transform(AddComponent<CTransform>())
{
	OVLOG( "scene " + std::to_string(p_scene->GetSceneId()) +  " Create Actor " + std::to_string(p_actorID) + "  name =" + p_name);
	CreatedEvent.Invoke(*this);
}

LittleEngine::Actor::~Actor()
{
	
}

const std::string & LittleEngine::Actor::GetName() const
{
	return m_name;
}

const std::string & LittleEngine::Actor::GetTag() const
{
	return m_tag;
}

void LittleEngine::Actor::SetName(const std::string & p_name)
{
	m_name = p_name;
}

void LittleEngine::Actor::SetTag(const std::string & p_tag)
{
	m_tag = p_tag;
}

void LittleEngine::Actor::SetActive(bool p_active)
{
	if (p_active != m_active)
	{
		RecursiveWasActiveUpdate();
		m_active = p_active;
		RecursiveActiveUpdate();
	}
}

bool LittleEngine::Actor::IsSelfActive() const
{
	return m_active;
}

bool LittleEngine::Actor::IsActive() const
{
	return m_active && (m_parent ? m_parent->IsActive() : true);
}

void LittleEngine::Actor::SetID(int64_t p_id)
{
	m_actorID = p_id;
}

int64_t LittleEngine::Actor::GetID() const
{
	return m_actorID;
}

void LittleEngine::Actor::SetParent(Actor& p_parent)
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

void LittleEngine::Actor::DetachFromParent()
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

bool LittleEngine::Actor::HasParent() const
{
	return m_parent;
}

LittleEngine::Actor * LittleEngine::Actor::GetParent() const
{
	return m_parent;
}

int64_t LittleEngine::Actor::GetParentID() const
{
	return m_parentID;
}

std::vector<LittleEngine::Actor*>& LittleEngine::Actor::GetChildren()
{
	return m_children;
}
bool LittleEngine::Actor::Destroy()
{
	if(!IsAlive()) return false;
	Owner->DestroyActor(*this);
	for (auto child : m_children)
		child->Destroy();
	return true;
}

void LittleEngine::Actor::MarkAsDestroying()
{
	m_aliveState = EActorAliveState::Destroying;
}
void LittleEngine::Actor::MarkAsDestroyed()
{
	m_aliveState = EActorAliveState::Destroyed;
}
bool LittleEngine::Actor::IsAlive() const
{
	return m_aliveState == EActorAliveState::Alive ;
}
bool LittleEngine::Actor::IsDestroying()const
{
	return m_aliveState == EActorAliveState::Destroying ;
}
bool LittleEngine::Actor::IsDestroyed()const
{
	return m_aliveState == EActorAliveState::Destroyed ;
}
void LittleEngine::Actor::SetSleeping(bool p_sleeping)
{
	m_sleeping = p_sleeping;
}

std::vector<std::shared_ptr<LittleEngine::AComponent>>& LittleEngine::Actor::GetComponentsCopy(
	std::vector<std::shared_ptr<LittleEngine::AComponent>>& comps)
{
	m_tempComponents.clear();
	for (auto ident : comps){
		m_tempComponents.push_back(ident);
	}
	return m_tempComponents;
}

void LittleEngine::Actor::OnAwake()
{
	if(m_awaked) return;
	m_awaked = true;
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnAwake(); });
}

void LittleEngine::Actor::OnStart()
{
	if(m_started) return;
	m_started = true;
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnStart(); });
}

void LittleEngine::Actor::OnEnable()
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnEnable(); });
}

void LittleEngine::Actor::OnDisable()
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnDisable(); });
}

void LittleEngine::Actor::OnDestroy()
{
	if (!m_sleeping)
	{
		if (IsActive())
			OnDisable();
	}
	DestroyedEvent.Invoke(*this);
	
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [](auto element) { element->OnDestroy(); });
	std::for_each(comps.begin(), comps.end(), [&](std::shared_ptr<AComponent> p_component) { ComponentRemovedEvent.Invoke(*p_component); });

	DetachFromParent();
	
	MarkAsDestroyed();
	m_components.clear();
	m_tempComponents.clear();
	m_children.clear();
}

void LittleEngine::Actor::OnUpdate(float p_deltaTime)
{
	if (IsAlive()&& IsActive())
	{
		auto comps = GetComponentsCopy(m_components);
		std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnUpdate(p_deltaTime); });
	}
}

void LittleEngine::Actor::OnFixedUpdate(float p_deltaTime)
{
	if (IsAlive()&&IsActive())
	{
		auto comps = GetComponentsCopy(m_components);
		std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnFixedUpdate(p_deltaTime); });
	}
}

void LittleEngine::Actor::OnLateUpdate(float p_deltaTime)
{
	if (IsAlive()&&IsActive())
	{
		auto comps = GetComponentsCopy(m_components);
		std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnLateUpdate(p_deltaTime); });
	}
}

void LittleEngine::Actor::OnCollisionEnter(CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionEnter(p_otherObject); });
}

void LittleEngine::Actor::OnCollisionStay(CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionStay(p_otherObject); });
}

void LittleEngine::Actor::OnCollisionExit(CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnCollisionExit(p_otherObject); });
}

void LittleEngine::Actor::OnTriggerEnter(CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerEnter(p_otherObject); });
}

void LittleEngine::Actor::OnTriggerStay(CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerStay(p_otherObject); });
}

void LittleEngine::Actor::OnTriggerExit(CPhysicalObject& p_otherObject)
{
	auto comps = GetComponentsCopy(m_components);
	std::for_each(comps.begin(), comps.end(), [&](auto element) { element->OnTriggerExit(p_otherObject); });
}

bool LittleEngine::Actor::RemoveComponent(LittleEngine::AComponent& p_component)
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

std::vector<std::shared_ptr<LittleEngine::AComponent>>& LittleEngine::Actor::GetComponents()
{
	return m_components;
}


void LittleEngine::Actor::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_actorsRoot)
{
	tinyxml2::XMLNode* actorNode = p_doc.NewElement("actor");
	p_actorsRoot->InsertEndChild(actorNode);

	LittleEngine::Serializer::SerializeString(p_doc, actorNode, "name", m_name);
	LittleEngine::Serializer::SerializeString(p_doc, actorNode, "tag", m_tag);
	LittleEngine::Serializer::SerializeBoolean(p_doc, actorNode, "active", m_active);
	LittleEngine::Serializer::SerializeInt64(p_doc, actorNode, "id", m_actorID);
	LittleEngine::Serializer::SerializeInt64(p_doc, actorNode, "parent", m_parentID);

	tinyxml2::XMLNode* componentsNode = p_doc.NewElement("components");
	actorNode->InsertEndChild(componentsNode);

	for (auto& component : m_components)
	{
		/* Current component root */
		tinyxml2::XMLNode* componentNode = p_doc.NewElement("component");
		componentsNode->InsertEndChild(componentNode);

		/* Component type */
		LittleEngine::Serializer::SerializeString(p_doc, componentNode, "type", typeid(*component).name());

		/* Data node (Will be passed to the component) */
		tinyxml2::XMLElement* data = p_doc.NewElement("data");
		componentNode->InsertEndChild(data);

		/* Data serialization of the component */
		component->OnSerialize(p_doc, data);
	}

	tinyxml2::XMLNode* behavioursNode = p_doc.NewElement("behaviours");
	actorNode->InsertEndChild(behavioursNode);
}
void LittleEngine::Actor::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_actorsRoot)
{
	LittleEngine::Serializer::DeserializeString(p_doc, p_actorsRoot, "name", m_name);
	LittleEngine::Serializer::DeserializeString(p_doc, p_actorsRoot, "tag", m_tag);
	LittleEngine::Serializer::DeserializeBoolean(p_doc, p_actorsRoot, "active", m_active);
	LittleEngine::Serializer::DeserializeInt64(p_doc, p_actorsRoot, "id", m_actorID);
	LittleEngine::Serializer::DeserializeInt64(p_doc, p_actorsRoot, "parent", m_parentID);

	{
		tinyxml2::XMLNode* componentsRoot = p_actorsRoot->FirstChildElement("components");
		if (componentsRoot)
		{
			tinyxml2::XMLElement* currentComponent = componentsRoot->FirstChildElement("component");

			while (currentComponent)
			{
				std::string componentType = currentComponent->FirstChildElement("type")->GetText();
				LittleEngine::AComponent* component = nullptr;

				// TODO: Use component name instead of typeid (unsafe)
				if (componentType == typeid(CTransform).name())			component = &transform;
				else if (componentType == typeid(CPhysicalBox).name())			component = &AddComponent<LittleEngine::CPhysicalBox>();
				else if (componentType == typeid(CPhysicalSphere).name())		component = &AddComponent<LittleEngine::CPhysicalSphere>();
				else if (componentType == typeid(CPhysicalCapsule).name())		component = &AddComponent<LittleEngine::CPhysicalCapsule>();
				else if (componentType == typeid(CModelRenderer).name())			component = &AddComponent<LittleEngine::CModelRenderer>();
				else if (componentType == typeid(CCamera).name())				component = &AddComponent<LittleEngine::CCamera>();
				else if (componentType == typeid(CMaterialRenderer).name())		component = &AddComponent<LittleEngine::CMaterialRenderer>();
				else if (componentType == typeid(CAudioSource).name())			component = &AddComponent<LittleEngine::CAudioSource>();
				else if (componentType == typeid(CAudioListener).name())		component = &AddComponent<LittleEngine::CAudioListener>();
				else if (componentType == typeid(CPointLight).name())			component = &AddComponent<LittleEngine::CPointLight>();
				else if (componentType == typeid(CDirectionalLight).name())		component = &AddComponent<LittleEngine::CDirectionalLight>();
				else if (componentType == typeid(CSpotLight).name())			component = &AddComponent<LittleEngine::CSpotLight>();
				else if (componentType == typeid(CAmbientBoxLight).name())		component = &AddComponent<LittleEngine::CAmbientBoxLight>();
				else if (componentType == typeid(CAmbientSphereLight).name())	component = &AddComponent<LittleEngine::CAmbientSphereLight>();
				else if (componentType == typeid(CAnimator).name())				component = &AddComponent<LittleEngine::CAnimator>();

				if (component)
					component->OnDeserialize(p_doc, currentComponent->FirstChildElement("data"));

				currentComponent = currentComponent->NextSiblingElement("component");
			}
		}
	}
}

void LittleEngine::Actor::RecursiveActiveUpdate()
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

void LittleEngine::Actor::RecursiveWasActiveUpdate()
{
	m_wasActive = IsActive();
	for (auto child : m_children)
		child->RecursiveWasActiveUpdate();
}
