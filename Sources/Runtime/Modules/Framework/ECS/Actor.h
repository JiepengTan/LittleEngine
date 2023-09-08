/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <unordered_map>
#include <memory>

#include "Core/Tools/Eventing/Event.h"

#include "Core/Base/Application.h"
#include "Core/CoreInclude.h"
#include "Modules/Framework/ECS/Component.h"
#include "Modules/Framework/ECS/Components/CTransform.h"
#include "Modules/Framework/ECS/Behaviour.h"
#include "Modules/Framework/API/ISerializable.h"
#include "Resource/ResIncludeScene.h"

namespace LittleEngine
{
	class Scene;
}

namespace LittleEngine
{
	enum EActorAliveState
	{
		Alive,
		Destroying,
		Destroyed
	};
	/**
	* The Actor is the main class of the ECS, it corresponds to the entity and is
	* composed of componenents and behaviours (scripts)
	*/
	REFLECTION_TYPE(Actor)
	CLASS (Actor :public Object , WhiteListFields)
	{
		
		REFLECTION_BODY(Actor)
		friend class Scene;
	public:

		Actor();
		/**
		* Constructor of the actor. It will automatically add a transform component
		* @param p_actorID
		* @param p_name
		* @param p_tag
		* @param p_playing
		*/
		void DoInit(Scene* p_scene, ActorID p_actorID, const std::string& p_name, const std::string& p_tag, bool& p_playing);

		/**
		* Destructor of the actor instance. Force invoke ComponentRemovedEvent and BehaviourRemovedEvent
		* for every components and behaviours
		*/
		virtual ~Actor() override;

		void SaveTo(ResActor& p_resActor); 
		/**
		* Return the current name of the actor
		*/
		const std::string& GetName() const;

		/**
		* Return the current tag of the actor
		*/
		const std::string& GetTag() const;

		/**
		* Defines a new name for the actor
		* @param p_name
		*/
		void SetName(const std::string& p_name);

		/**
		* Defines a new tag for the actor
		* @param p_tag
		*/
		void SetTag(const std::string& p_tag);

		/**
		* Enable or disable the actor
		* @param p_active
		*/
		void SetActive(bool p_active);

		/**
		* Returns true if the actor is active, ignoring his parent (if any) active state
		*/
		bool IsSelfActive() const;

		/**
		* Returns true if the actor is and his recursive parents (if any) are active
		*/
		bool IsActive() const;

		/**
		* Defines a new ID for the actor
		* @param p_id
		*/
		void SetID(ActorID p_id);

		/**
		* Returns the ID of the actor
		*/
		ActorID GetID() const;

		/**
		* Set an actor as the parent of this actor
		* @param p_parent
		*/
		void SetParent(ActorPtr p_parent);

		/**
		* Detach from the parent
		*/
		void DetachFromParent();

		/**
		* Returns true if the actor has a parent
		*/
		bool HasParent() const;

		/**
		* Returns the parents of this actor (Or nullptr if no parent)
		*/
		ActorPtr GetParent() const;

		/**
		* Returns the ID of the parent of this actor
		*/
		ActorID GetParentID() const;

		/**
		* Returns the children of this actor
		*/
		ActorVector GetChildren() const;

		bool Destroy();

		/**
		* Returns true if the actor is not marked as destroyed
		*/
		bool IsAlive() const;

		/**
		* Defines if the actor is sleeping or not.
		* A sleeping actor will not trigger methods suchs as OnEnable, OnDisable and OnDestroyed
		* @param p_sleeping
		*/
		void SetSleeping(bool p_sleeping);


		/**
		* Called when the actor enter in collision with another physical object
		* @param p_otherObject
		*/
		void OnCollisionEnter(SharedPtr<CPhysicalObject> p_otherObject);

		/**
		* Called when the actor is in collision with another physical object
		* @param p_otherObject
		*/
		void OnCollisionStay(SharedPtr<CPhysicalObject> p_otherObject);

		/**
		* Called when the actor exit from collision with another physical object
		* @param p_otherObject
		*/
		void OnCollisionExit(SharedPtr<CPhysicalObject> p_otherObject);

		/**
		* Called when the actor enter in trigger with another physical object
		* @param p_otherObject
		*/
		void OnTriggerEnter(SharedPtr<CPhysicalObject> p_otherObject);

		/**
		* Called when the actor is in trigger with another physical object
		* @param p_otherObject
		*/
		void OnTriggerStay(SharedPtr<CPhysicalObject> p_otherObject);

		/**
		* Called when the actor exit from trigger with another physical object
		* @param p_otherObject
		*/
		void OnTriggerExit(SharedPtr<CPhysicalObject> p_otherObject);

		/**
		* Add a component to the actor (Or return the component if already existing)
		* @param p_args (Parameter pack forwared to the component constructor)
		*/
		template<typename T>
		SharedPtr<T> AddComponent();

		/**
		* Remove the given component
		*/
		template<typename T>
		bool RemoveComponent();

		/**
		* Remove the component by refering to the given instance
		* @param p_component
		*/
		bool RemoveComponent(SharedPtr<Component> p_component);

		/**
		* Try to get the given component (Returns nullptr on failure)
		*/
		template<typename T>
		SharedPtr<T> GetComponent();
		
		/**
		* Returns a reference to the vector of components
		*/
		CompVector& GetComponentsInternal();


		/**
		* Serialize all the components
		*/
		virtual void OnSerialize(ISerializer p_serializer) override;

		/**
		* Deserialize all the components
		*/
		virtual void OnDeserialize(ISerializer p_serializer) override;

	private:
		/**
		 * @brief Deleted copy constructor
		 * @param p_actor
		 */
		Actor(const Actor& p_actor) = delete;

		void RecursiveActiveUpdate();
		void RecursiveWasActiveUpdate();

		ActorPtr GetSceneActor(ActorID p_actorID);
		void NotifyComponentAdd(CompPtr p_comp);
		void NotifyComponentRemoved(CompPtr p_comp);
	private:
		
		/**
		* Called when the scene start or when the actor gets enabled for the first time during play mode
		* This method will always be called in an ordered triple:
		* - OnAwake()
		* - OnEnable()
		* - OnStart()
		*/
		void OnAwake();

		/**
		* Called when the scene start or when the actor gets enabled for the first time during play mode
		* This method will always be called in an ordered triple:
		* - OnAwake()
		* - OnEnable()
		* - OnStart()
		*/
		void OnStart();

		/**
		* Called when the actor gets enabled (SetActive set to true) or at scene start if the actor is hierarchically active.
		* This method can be called in an ordered triple at scene start:
		* - OnAwake()
		* - OnEnable()
		* - OnStart()
		* Or can be called solo if the actor hierarchical active state changed to true and the actor already gets awaked
		* Conditions:
		* - Play mode only
		*/
		void OnEnable();

		/**
		* Called when the actor hierarchical active state changed to false or gets destroyed while being hierarchically active
		* Conditions:
		* - Play mode only
		*/
		void OnDisable();

		/**
		* Called when the actor gets destroyed if it has been awaked
		* Conditions:
		* - Play mode only
		*/
		void OnDestroy();

		/**
		* Called every frame
		* @param p_deltaTime
		*/
		void OnUpdate(float p_deltaTime);

		/**
		* Called every physics frame
		* @param p_deltaTime
		*/
		void OnFixedUpdate(float p_deltaTime);

		/**
		* Called every frame after OnUpdate
		* @param p_deltaTime
		*/
		void OnLateUpdate(float p_deltaTime);
		void MarkAsDestroying() ;
		void MarkAsDestroyed();
		bool IsDestroyed() const;
		bool IsDestroying() const;
		
		CompVector& GetComponentsCopy(CompMap& comps);
	public:
		SharedPtr<CTransform> transform;
	private:
		Scene* m_scene;
		META(Enable)
		std::string		m_prefabGuid = "";
		/* Settings */
		META(Enable)
		std::string		m_name = "";
		META(Enable)
		std::string		m_tag = "";
		META(Enable)
		bool			m_active = true;
		bool			m_playing = false;

		/* Internal settings */
		META(Enable)
		ActorID	m_actorID = 0;
		bool	m_sleeping = true;
		bool	m_awaked = false;
		bool	m_started = false;
		bool	m_wasActive = false;

		/* Parenting system stuff */
		META(Enable)
		ActorID				m_parentID = 0;
		TSet<ActorID>		m_childrenIds;

		/* Actors components */
		
		CompMap m_components;
		CompVector m_tempComponents;
		/**
		* Mark the Actor as "Destroyed". A "Destroyed" actor will be removed from the scene by the scene itself
		*/
		EActorAliveState m_aliveState= EActorAliveState::Alive ;
		
	};
}

#include "Modules/Framework/ECS/Actor.inl"
