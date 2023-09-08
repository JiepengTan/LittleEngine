/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "Modules/Framework/ECS/Actor.h"


namespace LittleEngine
{
	class CLight;
	class CCamera;
	class CModelRenderer;
	/**
	* The scene is a set of actors
	*/
	class Scene : Object
	{
	public:
		// events
		static Eventing::Event<ActorPtr>				 	DestroyedEvent;
		static Eventing::Event<ActorPtr>				 	CreatedEvent;
		static Eventing::Event<ActorPtr, ActorPtr>			AttachEvent;
		static Eventing::Event<ActorPtr>				 	DettachEvent;
	public:
		/**
		* Contains a set of vectors of components that are sorted. It allows fast
		* manipulation of components without parsing the whole scene
		*/
		struct FastAccessComponents
		{
			TUnorderedSet<ActorID>			modelRenderers;
			TUnorderedSet<ActorID>			cameras;
			TUnorderedSet<ActorID>			lights;

		public:
			void Clear()
			{
				modelRenderers.clear();
				cameras.clear();
				lights.clear();
			}
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
		ActorPtr CreateActor();

		/**
		* Create an actor with the given name and return a reference to it.
		* @param p_name
		* @param p_tag
		*/
		ActorPtr CreateActor(const std::string& p_name, const std::string& p_tag = "");

		/**
		* Destroy and actor and return true on success
		* @param p_target (The actor to remove from the scene)
		*/
		bool DestroyActor(Actor* p_target);
		bool DestroyActor(ActorID p_actorID);
		/**
		* Collect garbages by removing Destroyed-marked actors
		*/
		void CollectGarbages();

		/**
		* Return the first actor identified by the given name, or nullptr on fail
		* @param p_name
		*/
		ActorPtr FindActorByName(const std::string& p_name);

		/**
		* Return the first actor identified by the given tag, or nullptr on fail
		* @param p_tag
		*/
		ActorPtr FindActorByTag(const std::string& p_tag);

		/**
		* Return the actor identified by the given ID (Returns 0 on fail)
		* @param p_id
		*/
		ActorPtr FindActorByID(ActorID p_id);

		/**
		* Return every actors identified by the given name
		* @param p_name
		*/
		ActorVector FindActorsByName(const std::string& p_name);

		/**
		* Return every actors identified by the given tag
		* @param p_tag
		*/
		ActorVector FindActorsByTag(const std::string& p_tag);

		/**
		* Callback method called everytime a component is added on an actor of the scene
		* @param p_component
		*/
		void OnComponentAdded(CompPtr p_compononent);

		/**
		* Callback method called everytime a component is removed on an actor of the scene
		* @param p_component
		*/
		void OnComponentRemoved(CompPtr p_compononent);

		void OnActorDetach(ActorID p_child,ActorID p_parent);
		void OnActorAttach(ActorID p_child,ActorID p_parent);
		/**
		* Return a reference on the actor map
		*/
		ActorVector& GetActorsInternal();

		/**
		* Return the fast access components data structure
		*/
		const FastAccessComponents& GetFastAccessComponents() const;
		TVector<SharedPtr<CLight>> GetLights() const;
		TVector<SharedPtr<CCamera>> GetCameras() const;
		TVector<SharedPtr<CModelRenderer>> GetRenderers() const;
		template<typename T>
		TVector<SharedPtr<T>> GetComponents(TUnorderedSet<ActorID> p_IDs) const 
		{
			TVector<SharedPtr<T>> vector;
			for (auto ident : p_IDs){
				auto actor = GetActor(ident);
				if(actor != nullptr)
				{
					vector.push_back(actor->GetComponent<T>());
				}
			}
			return vector;
		}
		
		/**
		* Serialize the scene
		* @param p_doc
		* @param p_root
		*/
		virtual void OnSerialize(ISerializer p_serializer) override;

		/**
		* Deserialize the scene
		* @param p_doc
		* @param p_root
		*/
		virtual void OnDeserialize(ISerializer p_serializer) override;

		ActorVector& GetActorsCopy(ActorMap& p_vec);
		ActorVector& GetActorsCopy(ActorVector& p_vec);
		bool HasActor(ActorID p_actorId)const;
		ActorPtr GetActor(ActorID p_actorId)const;
		ObjectID GetSceneId()const;

	private:
		bool m_isPlaying = false;
		
		ActorMap m_actors;
		ActorVector m_tempActors;
		ActorVector m_tempDeleteActors;

		FastAccessComponents m_fastAccessComponents;
		ObjectID m_sceneId;
	private:
		static int s_globalSceneId; 
	};
}
