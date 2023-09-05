/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>
#include <string>

#include "Modules/Framework/SceneSystem/Scene.h"

int LittleEngine::SceneSystem::Scene::s_globalSceneId = 1; 
LittleEngine::SceneSystem::Scene::Scene()
{
	m_sceneId = s_globalSceneId++;
}

LittleEngine::SceneSystem::Scene::~Scene()
{
	std::for_each(m_actors.begin(), m_actors.end(), [](LittleEngine::Actor* element)
	{ 
		delete element;
	});

	m_actors.clear();
}

void LittleEngine::SceneSystem::Scene::Play()
{
	m_isPlaying = true;
	auto &actors = GetActorsCopy(m_actors);
	/* Wake up actors to allow them to react to OnEnable, OnDisable and OnDestroy, */
	std::for_each(actors.begin(), actors.end(), [](Actor * p_element) { p_element->SetSleeping(false); });
	
	std::for_each(actors.begin(), actors.end(), [](Actor * p_element) { if (p_element->IsActive()) p_element->OnAwake(); });
	std::for_each(actors.begin(), actors.end(), [](Actor * p_element) { if (p_element->IsActive()) p_element->OnEnable(); });
	std::for_each(actors.begin(), actors.end(), [](Actor * p_element) { if (p_element->IsActive()) p_element->OnStart(); });
}

bool LittleEngine::SceneSystem::Scene::IsPlaying() const
{
	return m_isPlaying;
}

void LittleEngine::SceneSystem::Scene::Update(float p_deltaTime)
{
	auto &actors = GetActorsCopy(m_actors);
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Actor::OnUpdate), std::placeholders::_1, p_deltaTime));
	CollectGarbages();
}

void LittleEngine::SceneSystem::Scene::FixedUpdate(float p_deltaTime)
{
	auto &actors = GetActorsCopy(m_actors);
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Actor::OnFixedUpdate), std::placeholders::_1, p_deltaTime));
	CollectGarbages();
}

void LittleEngine::SceneSystem::Scene::LateUpdate(float p_deltaTime)
{
	auto &actors = GetActorsCopy(m_actors);
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&Actor::OnLateUpdate), std::placeholders::_1, p_deltaTime));
	CollectGarbages();
}

LittleEngine::Actor& LittleEngine::SceneSystem::Scene::CreateActor()
{
	return CreateActor("New Actor");
}

LittleEngine::Actor& LittleEngine::SceneSystem::Scene::CreateActor(const std::string& p_name, const std::string& p_tag)
{
	m_actors.push_back(new LittleEngine::Actor(this,m_availableID++, p_name, p_tag, m_isPlaying));
	Actor& instance = *m_actors.back();
	instance.ComponentAddedEvent	+= std::bind(&Scene::OnComponentAdded, this, std::placeholders::_1);
	instance.ComponentRemovedEvent	+= std::bind(&Scene::OnComponentRemoved, this, std::placeholders::_1);
	if (m_isPlaying)
	{
		instance.SetSleeping(false);
		if (instance.IsActive())
		{
			instance.OnAwake();
			instance.OnEnable();
			instance.OnStart();
		}
	}
	return instance;
}

bool LittleEngine::SceneSystem::Scene::DestroyActor(Actor& p_target)
{
	if(!p_target.IsAlive()) return false;
	p_target.MarkAsDestroying();
	OVLOG("DestroyActor Actor " + std::to_string(p_target.GetID()));
	m_tempDeleteActors.push_back(&p_target);
	p_target.OnDestroy();
	auto found = std::find_if(m_actors.begin(), m_actors.end(), [&p_target](LittleEngine::Actor* element)
	{
		return element == &p_target;
	});
	if (found != m_actors.end())
	{
		m_actors.erase(found);
		return true;
	}
	else
	{
		return false;
	}
}

void LittleEngine::SceneSystem::Scene::CollectGarbages()
{
	while (m_tempDeleteActors.size()>0)
	{
		auto&actors = GetActorsCopy(m_tempDeleteActors);
		m_tempDeleteActors.clear();
		for (auto actor : actors)
		{
			OVLOG("DestroyActor Collection " + std::to_string(actor->GetID()));
			delete actor;
		}
	}
}

LittleEngine::Actor* LittleEngine::SceneSystem::Scene::FindActorByName(const std::string& p_name)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_name](LittleEngine::Actor* element)
	{ 
		return element->GetName() == p_name;
	});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

LittleEngine::Actor* LittleEngine::SceneSystem::Scene::FindActorByTag(const std::string & p_tag)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_tag](LittleEngine::Actor* element)
	{
		return element->GetTag() == p_tag;
	});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

LittleEngine::Actor* LittleEngine::SceneSystem::Scene::FindActorByID(int64_t p_id)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_id](LittleEngine::Actor* element)
	{
		return element->GetID() == p_id;
	});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

std::vector<std::reference_wrapper<LittleEngine::Actor>> LittleEngine::SceneSystem::Scene::FindActorsByName(const std::string & p_name)
{
	std::vector<std::reference_wrapper<LittleEngine::Actor>> actors;

	for (auto actor : m_actors)
	{
		if (actor->GetName() == p_name)
			actors.push_back(std::ref(*actor));
	}

	return actors;
}

std::vector<std::reference_wrapper<LittleEngine::Actor>> LittleEngine::SceneSystem::Scene::FindActorsByTag(const std::string & p_tag)
{
	std::vector<std::reference_wrapper<LittleEngine::Actor>> actors;

	for (auto actor : m_actors)
	{
		if (actor->GetTag() == p_tag)
			actors.push_back(std::ref(*actor));
	}

	return actors;
}

void LittleEngine::SceneSystem::Scene::OnComponentAdded(Component& p_compononent)
{
	if (auto result = dynamic_cast<CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.push_back(result);

	if (auto result = dynamic_cast<CCamera*>(&p_compononent))
		m_fastAccessComponents.cameras.push_back(result);

	if (auto result = dynamic_cast<CLight*>(&p_compononent))
		m_fastAccessComponents.lights.push_back(result);
}

void LittleEngine::SceneSystem::Scene::OnComponentRemoved(Component& p_compononent)
{
	if (auto result = dynamic_cast<CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.erase(std::remove(m_fastAccessComponents.modelRenderers.begin(), m_fastAccessComponents.modelRenderers.end(), result), m_fastAccessComponents.modelRenderers.end());

	if (auto result = dynamic_cast<CCamera*>(&p_compononent))
		m_fastAccessComponents.cameras.erase(std::remove(m_fastAccessComponents.cameras.begin(), m_fastAccessComponents.cameras.end(), result), m_fastAccessComponents.cameras.end());

	if (auto result = dynamic_cast<CLight*>(&p_compononent))
		m_fastAccessComponents.lights.erase(std::remove(m_fastAccessComponents.lights.begin(), m_fastAccessComponents.lights.end(), result), m_fastAccessComponents.lights.end());
}

std::vector<LittleEngine::Actor*>& LittleEngine::SceneSystem::Scene::GetActors()
{
	return m_actors;
}

const LittleEngine::SceneSystem::Scene::FastAccessComponents& LittleEngine::SceneSystem::Scene::GetFastAccessComponents() const
{
	return m_fastAccessComponents;
}

void LittleEngine::SceneSystem::Scene::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_root)
{
	tinyxml2::XMLNode* sceneNode = p_doc.NewElement("scene");
	p_root->InsertEndChild(sceneNode);

	tinyxml2::XMLNode* actorsNode = p_doc.NewElement("actors");
	sceneNode->InsertEndChild(actorsNode);

	for (auto& actor : m_actors)
	{
		actor->OnSerialize(p_doc, actorsNode);
	}
}

std::vector<LittleEngine::Actor*>& LittleEngine::SceneSystem::Scene::GetActorsCopy(std::vector<Actor*>& p_vec)
{
	m_tempActors.clear();
	for (auto item : p_vec)
		m_tempActors.push_back(item);
	return m_tempActors;
}
void LittleEngine::SceneSystem::Scene::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_root)
{
	tinyxml2::XMLNode* actorsRoot = p_root->FirstChildElement("actors");

	if (actorsRoot)
	{
		tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");

		int64_t maxID = 1;

		while (currentActor)
		{
			auto& actor = CreateActor();
			actor.OnDeserialize(p_doc, currentActor);
			maxID = std::max(actor.GetID() + 1, maxID);
			currentActor = currentActor->NextSiblingElement("actor");
		}

		m_availableID = maxID;

		/* We recreate the hierarchy of the scene by attaching children to their parents */
		for (auto actor : m_actors)
		{
			if (actor->GetParentID() > 0)
			{
				if (auto found = FindActorByID(actor->GetParentID()); found)
					actor->SetParent(*found);
			}
		}
	}
}
