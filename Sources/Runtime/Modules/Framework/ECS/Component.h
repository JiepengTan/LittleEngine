/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/CoreInclude.h"
#include "Modules/Framework/API/IInspectorItem.h"

namespace LittleEngine
{
	class Actor;
	class CPhysicalObject;
	/**
	* AComponent is the base class for any component.
	* A component is a set of data and behaviours (Entity-Component without systems) that is interpreted by the engine (Or the user)
	*/
	REFLECTION_COMPONENT_TYPE(Component)
	CLASS (Component: public Object, WhiteListFields,Abstract)
	{
		REFLECTION_COMPONENT_BODY(Component)
		friend class Actor;
	protected:
		TypeID _LE_InternalMetaTypeID;// internal type id for reflection
		void _LE_InitDebugInfo();
#if DEBUG
		std::string _LE_InternalMetaTypeName;
#endif
	public:
		/**
		* Destroying a AComponent will call dervied classes destructors
		*/
		virtual ~Component();
		virtual void OnBeforeSceneSave(ActorPtr p_actor){}
		virtual void OnAfterSceneLoaded(ActorPtr p_actor){}
		
		/**
		* Constructor of a AComponent (Must be called by derived classes)
		* @param p_owner
		*/
		virtual void OnConstruction(){}
		
		/**
		* Called when the scene start right before OnStart
		* It allows you to apply prioritized game logic on scene start
		*/
		virtual void OnAwake() {}

		/**
		* Called when the scene start right after OnAwake
		* It allows you to apply prioritized game logic on scene start
		*/
		virtual void OnStart() {}

		/**
		* Called when the components gets enabled (owner SetActive set to true) and after OnAwake() on scene starts
		*/
		virtual void OnEnable() {}

		/**
		* Called when the component gets disabled (owner SetActive set to false) and before OnDestroy() when the component get destroyed
		*/
		virtual void OnDisable() {}

		/**
		* Called when the components gets destroyed
		*/
		virtual void OnDestroy() {}

		/**
		* Called every frame
		* @param p_deltaTime
		*/
		virtual void OnUpdate(float p_deltaTime) {}

		/**
		* Called every physics frame
		* @param p_deltaTime
		*/
		virtual void OnFixedUpdate(float p_deltaTime) {}

		/**
		* Called every frame after OnUpdate
		* @param p_deltaTime
		*/
		virtual void OnLateUpdate(float p_deltaTime) {}

		/**
		* Called when the owner of this component enter in collision with another physical object
		* @param p_otherObject
		*/
		virtual void OnCollisionEnter(SharedPtr<CPhysicalObject> p_otherObject) {}

		/**
		* Called when the owner of this component is in collision with another physical object
		* @param p_otherObject
		*/
		virtual void OnCollisionStay(SharedPtr<CPhysicalObject> p_otherObject){}

		/**
		* Called when the owner of this component exit from collision with another physical object
		* @param p_otherObject
		*/
		virtual void OnCollisionExit(SharedPtr<CPhysicalObject> p_otherObject) {}

		/**
		* Called when the owner of this component enter in trigger with another physical object
		* @param p_otherObject
		*/
		virtual void OnTriggerEnter(SharedPtr<CPhysicalObject> p_otherObject) {}

		/**
		* Called when the owner of this component is in trigger with another physical object
		* @param p_otherObject
		*/
		virtual void OnTriggerStay(SharedPtr<CPhysicalObject> p_otherObject) {}

		/**
		* Called when the owner of this component exit from trigger with another physical object
		* @param p_otherObject
		*/
		virtual void OnTriggerExit(SharedPtr<CPhysicalObject> p_otherObject) {}
		
		/**
		* Returns the name of the component
		*/
		virtual std::string GetName() {return "Error Component!";}
		ActorPtr GetActor() {return owner;}
		void SetActor(ActorPtr actor);
		
		virtual void OnInspector(){}
		virtual void OnSerialize(ISerializer p_serializer) override;
		virtual void OnDeserialize(ISerializer p_serializer) override;
		virtual void OnInspectorGUI();
	public:
        META(Enable)
		bool isUpdateInEdit = false;
	protected:
		ActorPtr owner;
	};
}
