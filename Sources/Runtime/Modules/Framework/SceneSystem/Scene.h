/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/API/ISerializable.h"

#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Components/CLight.h"

namespace LittleEngine::SceneSystem
{
	/**
	* The scene is a set of actors
	*/
	class Scene : public API::ISerializable
	{
	public:
		/**
		* Contains a set of vectors of components that are sorted. It allows fast
		* manipulation of components without parsing the whole scene
		*/
		struct FastAccessComponents
		{
			std::vector<CModelRenderer*>	modelRenderers;
			std::vector<CCamera*>			cameras;
			std::vector<CLight*>			lights;
		};

		/**
		* Constructor of the scene
		*/
		Scene();

		/**
		* Handle the memory de-allocation of every actors
		*/
		~Scene();

		/**
		* Play the scene
		*/
		void Play();

		/**
		* Returns true if the scene is playing
		*/
		bool IsPlaying() const;

		/**
		* Update every actors
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Update every actors 60 frames per seconds
		* @param p_deltaTime
		*/
		void FixedUpdate(float p_deltaTime);

		/**
		* Update every actors lately
		* @param p_deltaTime
		*/
		void LateUpdate(float p_deltaTime);

		/**
		* Create an actor with a default name and return a reference to it.
		*/
		Actor& CreateActor();

		/**
		* Create an actor with the given name and return a reference to it.
		* @param p_name
		* @param p_tag
		*/
		Actor& CreateActor(const std::string& p_name, const std::string& p_tag = "");

		/**
		* Destroy and actor and return true on success
		* @param p_target (The actor to remove from the scene)
		*/
		bool DestroyActor(Actor& p_target);

		/**
		* Collect garbages by removing Destroyed-marked actors
		*/
		void CollectGarbages();

		/**
		* Return the first actor identified by the given name, or nullptr on fail
		* @param p_name
		*/
		Actor* FindActorByName(const std::string& p_name);

		/**
		* Return the first actor identified by the given tag, or nullptr on fail
		* @param p_tag
		*/
		Actor* FindActorByTag(const std::string& p_tag);

		/**
		* Return the actor identified by the given ID (Returns 0 on fail)
		* @param p_id
		*/
		Actor* FindActorByID(int64_t p_id);

		/**
		* Return every actors identified by the given name
		* @param p_name
		*/
		std::vector<std::reference_wrapper<Actor>> FindActorsByName(const std::string& p_name);

		/**
		* Return every actors identified by the given tag
		* @param p_tag
		*/
		std::vector<std::reference_wrapper<Actor>> FindActorsByTag(const std::string& p_tag);

		/**
		* Callback method called everytime a component is added on an actor of the scene
		* @param p_component
		*/
		void OnComponentAdded(Component& p_compononent);

		/**
		* Callback method called everytime a component is removed on an actor of the scene
		* @param p_component
		*/
		void OnComponentRemoved(Component& p_compononent);

		/**
		* Return a reference on the actor map
		*/
		std::vector<LittleEngine::Actor*>& GetActors();

		/**
		* Return the fast access components data structure
		*/
		const FastAccessComponents& GetFastAccessComponents() const;

		/**
		* Serialize the scene
		* @param p_doc
		* @param p_root
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root) override;

		/**
		* Deserialize the scene
		* @param p_doc
		* @param p_root
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root) override;

		std::vector<Actor*>& GetActorsCopy(std::vector<Actor*>& p_vec);

		int GetSceneId(){ return m_sceneId;}
	private:
		int64_t m_availableID = 1;
		bool m_isPlaying = false;
		std::vector<Actor*> m_actors;
		std::vector<Actor*> m_tempActors;
		std::vector<Actor*> m_tempDeleteActors;

		FastAccessComponents m_fastAccessComponents;
		int m_sceneId;
	private:
		static int s_globalSceneId; 
	};
}
