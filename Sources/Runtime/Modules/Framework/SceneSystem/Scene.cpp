/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "algorithm"
#include "string"

#include "Modules/Framework/SceneSystem/Scene.h"

#include "Core/Tools/Filesystem/FileUtil.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Components/CLight.h"
#include "Resource/Data/ResIncludeScene.h"
#include "_Generated/Serializer/AllSerializer.h"

namespace LittleEngine
{
    int Scene::s_globalSceneId = 1;

    Eventing::Event<ActorPtr> Scene::DestroyedEvent;
    Eventing::Event<ActorPtr> Scene::CreatedEvent;
    Eventing::Event<ActorPtr, ActorPtr> Scene::AttachEvent;
    Eventing::Event<ActorPtr> Scene::DettachEvent;
    
    Scene::Scene()
    {
        m_sceneId = ObjectIDAllocator::Alloc();
    }

    Scene::~Scene()
    {
        m_fastAccessComponents.Clear();
        m_tempDeleteActors.clear();
        m_tempActors.clear();
        m_actors.clear();
    }

    void Scene::Play()
    {
        m_isPlaying = true;
        auto& actors = GetActorsCopy(m_actors);
        /* Wake up actors to allow them to react to OnEnable, OnDisable and OnDestroy, */
        for (auto& item : actors)
            item->SetSleeping(false);   
        for (auto& item : actors)
            item->OnAwake();   
        for (auto& item : actors)
            item->OnEnable();   
        for (auto& item : actors)
            item->OnStart();   
    }

    bool Scene::IsPlaying() const
    {
        return m_isPlaying;
    }

    void Scene::Update(float p_deltaTime)
    {
        auto& actors = GetActorsCopy(m_actors);
        for (auto& item : actors)
            item->OnUpdate(p_deltaTime);   
        CollectGarbages();
    }

    void Scene::FixedUpdate(float p_deltaTime)
    {
        auto& actors = GetActorsCopy(m_actors);
        for (auto& item : actors)
            item->OnFixedUpdate(p_deltaTime);   
        CollectGarbages();
    }

    void Scene::LateUpdate(float p_deltaTime)
    {
        auto& actors = GetActorsCopy(m_actors);
        for (auto& item : actors)
            item->OnLateUpdate(p_deltaTime);   
        CollectGarbages();
    }

    ActorPtr Scene::CreateActor()
    {
        return CreateActor("New Actor");
    }

    ActorPtr Scene::CreateActor(const std::string& p_name, const std::string& p_tag)
    {
        auto actorId = ActorIDAllocator::Alloc();
        LOG_INFO("scene " + std::to_string(GetSceneId()) + " Create Actor " + std::to_string(actorId) + "  name ="            + p_name);
        auto instance= MakeSharedPtr<Actor>();
        instance->SetID(actorId);
        m_actors.emplace(instance->GetID(), instance);
        instance->DoInit(this, actorId, p_name, p_tag, m_isPlaying);
        CreatedEvent.Invoke(instance);
        
        if (m_isPlaying)
        {
            instance->SetSleeping(false);
            if (instance->IsActive())
            {
                instance->OnAwake();
                instance->OnEnable();
                instance->OnStart();
            }
        }
        return instance;
    }
    bool Scene::DestroyActor(ActorID p_actorID)
    {
        auto ptr = GetActor(p_actorID);
        if(ptr == nullptr) return false;
        return DestroyActor(ptr.get());
    }
    bool Scene::DestroyActor(Actor* p_target)
    {
        if(p_target == nullptr) return false;
        if(m_actors.count(p_target->GetID()) == 0) return false;
        if (!p_target->IsAlive()) return false;
        p_target->MarkAsDestroying();
        LOG_INFO("DestroyActor Actor " + std::to_string(p_target->GetID()));
        auto actor = m_actors[p_target->GetID()];
        m_tempDeleteActors.push_back(actor);
        actor->OnDestroy();
        DestroyedEvent.Invoke(actor);
        m_actors.erase(actor->GetID());
        return true;
    }

    void Scene::CollectGarbages()
    {
        while (m_tempDeleteActors.size() > 0)
        {
            auto& actors = GetActorsCopy(m_tempDeleteActors);
            m_tempDeleteActors.clear();
            for (auto actor : actors)
            {
                LOG_INFO("DestroyActor Collection " + std::to_string(actor->GetID()));
                //delete actor;
            }
            actors.clear();
        }
    }

    ActorPtr Scene::FindActorByName(const std::string& p_name)
    {
        for (auto& ident : m_actors)
        {
            auto actor = ident.second;
            if(actor!= nullptr && actor->GetName() == p_name)
                return actor;
        }
        return nullptr;
    }
    ActorVector Scene::FindActorsByName(const std::string& p_name)
    {
        ActorVector actors;
        for (auto item : m_actors)
        {
            if (item.second->GetName() == p_name)
                actors.push_back(item.second);
        }
        return actors;
    }

    ActorPtr Scene::FindActorByTag(const std::string& p_tag)
    {
        for (auto& item : m_actors)
        {
             auto actor = item.second;
             if(actor!= nullptr && actor->GetName() == p_tag)
                return actor;
        }
        return nullptr;
      
    }
    ActorVector Scene::FindActorsByTag(const std::string& p_tag)
    {
        ActorVector actors;
        for (auto item : m_actors)
        {
            if (item.second->GetName() == p_tag)
                actors.push_back(item.second);
        }

        return actors;
    }
    
    ActorPtr Scene::FindActorByID(ActorID p_id)
    {
        if(m_actors.count(p_id) == 0)
            return nullptr;
        return m_actors.at(p_id);
    }



    void Scene::OnActorDetach(ActorID p_child,ActorID p_parent)
    {
        auto child = GetActor(p_child);
        DettachEvent.Invoke(child);
    }
    void Scene::OnActorAttach(ActorID p_child,ActorID p_parent)
    {
        auto child = GetActor(p_child);
        auto parent = GetActor(p_parent);
        AttachEvent.Invoke(child,parent);
    }
    
    void Scene::OnComponentAdded(CompPtr p_compononent)
    {
        if(p_compononent->IsAssignableFrom(CModelRenderer::GetStaticTypeID()))
            m_fastAccessComponents.modelRenderers.insert(p_compononent->GetActor()->GetID());
        if(p_compononent->IsAssignableFrom(CCamera::GetStaticTypeID()))
            m_fastAccessComponents.cameras.insert(p_compononent->GetActor()->GetID());
        if(p_compononent->IsAssignableFrom(CLight::GetStaticTypeID()))
            m_fastAccessComponents.lights.insert(p_compononent->GetActor()->GetID());
    }

    void Scene::OnComponentRemoved(CompPtr p_compononent)
    {
        if(p_compononent->IsAssignableFrom(CModelRenderer::GetStaticTypeID()))
            m_fastAccessComponents.modelRenderers.erase(p_compononent->GetActor()->GetID());
        if(p_compononent->IsAssignableFrom(CCamera::GetStaticTypeID()))
            m_fastAccessComponents.cameras.erase(p_compononent->GetActor()->GetID());
        if(p_compononent->IsAssignableFrom(CLight::GetStaticTypeID()))
            m_fastAccessComponents.lights.erase(p_compononent->GetActor()->GetID());
    }

    ActorVector& Scene::GetActorsInternal()
    {
        return GetActorsCopy(m_actors);
    }

    const Scene::FastAccessComponents& Scene::GetFastAccessComponents() const
    {
        return m_fastAccessComponents;
    }

    
    TVector<SharedPtr<CLight>> Scene::GetLights() const
    {
        return GetComponents<CLight>(m_fastAccessComponents.lights);
    }
    TVector<SharedPtr<CCamera>> Scene::GetCameras() const
    {
        return GetComponents<CCamera>(m_fastAccessComponents.cameras);
    }
    TVector<SharedPtr<CModelRenderer>> Scene::GetRenderers() const
    {
        return GetComponents<CModelRenderer>(m_fastAccessComponents.modelRenderers);
    }
    
    ActorVector& Scene::GetActorsCopy(ActorMap& p_vec)
    {
        m_tempActors.clear();
        for (auto item : p_vec)
            m_tempActors.push_back(item.second);
        return m_tempActors;
    }
    ActorVector& Scene::GetActorsCopy(ActorVector& p_vec)
    {
        m_tempActors.clear();
        for (auto item : p_vec)
            m_tempActors.push_back(item);
        return m_tempActors;
    }

    bool Scene::HasActor(ActorID p_actorId) const    { return m_actors.count(p_actorId) != 0;}

    ActorPtr Scene::GetActor(ActorID p_actorId) const
    {
        if( m_actors.count(p_actorId) == 0) return nullptr;
        //LE_ASSERT(m_actors.count(p_actorId) != 0,"Can not find a actor in scene, should call HasActor first??" + std::to_string(p_actorId));
        return m_actors.at(p_actorId);
    }

    ObjectID Scene::GetSceneId() const    { return m_sceneId;}
    void Scene::LoadFrom(ResScene& resScene)
    {
        for (auto& resActor : resScene.m_actors)
        {
            auto actor= MakeSharedPtr<Actor>();
            actor->m_scene = this; 
            actor->LoadFrom(resActor);
            m_actors.emplace(actor->GetID(), actor);
        }
        ActorID maxActorId = 0;
        for (auto& resActor : resScene.m_actors)
        {
            maxActorId = std::max(maxActorId,resActor.m_actorID);
        }
        ActorIDAllocator::Init(maxActorId);
        
        auto actors = GetActorsCopy(m_actors);
        // update transform
        std::queue<ActorPtr> rootActors;
        for (auto actor : actors)
        {
            if(!HasActor(actor->m_parentID))
            {
                actor->m_parentID = k_invalidID;
                rootActors.emplace(actor);
            }
        }
        while (rootActors.size()>0)
        {
            auto& actor = rootActors.front();
            rootActors.pop();
            if(actor->m_parentID == k_invalidID)
            {
                actor->transform->OnSceneLoadedUpdate(nullptr);
            }else
            {
                actor->transform->OnSceneLoadedUpdate(GetActor(actor->m_parentID)->transform);
            }
            for (auto child : actor->m_childrenIds)
            {
                rootActors.push(GetActor(child));
            }
        }
        // call child to resolve references
        for (auto actor : actors)
        {
            actor->OnAfterSceneLoaded(this);
        }
        // update components cache
        for (auto actor : actors)
        {
            auto comps = actor->GetComponentsInternal();
            for (auto component : comps)
            {
                OnComponentAdded(component);
            }
        }
    }
    void Scene::SaveTo(ResScene& resScene)
    {
        auto& actors = GetActorsCopy(m_actors);
        auto count = actors.size();
        resScene.m_actors.reserve(count);
        for (auto item : actors)
        {
            ResActor actor;
            item->SaveTo(actor);
            resScene.m_actors.push_back(actor);
        }
    }
    void Scene::OnSerialize(ISerializer p_serializer)
    {
        // TODO tanjp OnSerialize Scene
    }
    void Scene::OnDeserialize(ISerializer p_serializer)
    {
        //1. create Actors
        //2. resolve actor's references
        //update global states
        //LOG_INFO(" maxID " +  maxID);
        /* We recreate the hierarchy of the scene by attaching children to their parents */
    }
}
