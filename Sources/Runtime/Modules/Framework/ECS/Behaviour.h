/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "sol.hpp"

#include "Modules/Framework/ECS/Component.h"
#include "Modules/Framework/ECS/Components/CPhysicalObject.h"
#include "Modules/Framework/ECS/Components/CTransform.h"

namespace LittleEngine
{
	class Actor;
}

namespace LittleEngine
{
	/**
	* ABehaviour is the base class for any behaviour.
	* A Behaviour is a script that is used to manipulate an actor over time
	*/
	REFLECTION_TYPE(Behaviour)
	CLASS (Behaviour: public Component, WhiteListFields)
	{
		REFLECTION_BODY(Behaviour)
	public:
		
		Behaviour() = default;

		/**
		* Destructor
		*/
		~Behaviour();

		/**
		* Returns the name of the component
		*/
		virtual std::string GetName() override;

		/**
		* Register the behaviour to lua
		* Returns true on success
		* @param p_luaState
		* @param p_rootFolder
		*/
		bool RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder);

		/**
		* Register the behaviour to lua
		* Returns true on success
		*/
		void UnregisterFromLuaContext();

		/**
		* Call a lua function for this behaviour
		* @param p_functionName
		* @param p_args
		*/
		template<typename... Args>
		void LuaCall(const std::string& p_functionName, Args&&... p_args);

		/**
		* Return the lua table attached to this behaviour
		*/
		sol::table& GetTable();

		/**
		* Called when the scene start right before OnStart
		* It allows you to apply prioritized game logic on scene start
		*/
		virtual void OnAwake() override;

		/**
		* Called when the scene start right after OnAwake
		* It allows you to apply prioritized game logic on scene start
		*/
		virtual void OnStart() override;

		/**
		* Called when the behaviour gets enabled (owner SetActive set to true)
		*/
		virtual void OnEnable() override;

		/**
		* Called when the behaviour gets disabled (owner SetActive set to false)
		*/
		virtual void OnDisable() override;

		/**
		* Called when the behaviour gets destroyed
		*/
		virtual void OnDestroy() override;

		/**
		* Called every frame
		* @param p_deltaTime
		*/
		virtual void OnUpdate(float p_deltaTime) override;

		/**
		* Called every physics frame
		* @param p_deltaTime
		*/
		virtual void OnFixedUpdate(float p_deltaTime) override;

		/**
		* Called every frame after OnUpdate
		* @param p_deltaTime
		*/
		virtual void OnLateUpdate(float p_deltaTime) override;

		/**
		* Called when the owner of this component enter in collision with another physical object
		* @param p_otherObject
		*/
		virtual void OnCollisionEnter(SharedPtr<CPhysicalObject> p_otherObject) override;

		/**
		* Called when the owner of this component is in collision with another physical object
		* @param p_otherObject
		*/
		virtual void OnCollisionStay(SharedPtr<CPhysicalObject> p_otherObject) override;

		/**
		* Called when the owner of this component exit from collision with another physical object
		* @param p_otherObject
		*/
		virtual void OnCollisionExit(SharedPtr<CPhysicalObject> p_otherObject) override;

		/**
		* Called when the owner of this component enter in trigger with another physical object
		* @param p_otherObject
		*/
		virtual void OnTriggerEnter(SharedPtr<CPhysicalObject> p_otherObject) override;

		/**
		* Called when the owner of this component is in trigger with another physical object
		* @param p_otherObject
		*/
		virtual void OnTriggerStay(SharedPtr<CPhysicalObject> p_otherObject) override;

		/**
		* Called when the owner of this component exit from trigger with another physical object
		* @param p_otherObject
		*/
		virtual void OnTriggerExit(SharedPtr<CPhysicalObject> p_otherObject) override;

		/**
		* Serialize the behaviour
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(ISerializer p_serializer) override;

		/**
		* Deserialize the behaviour
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(ISerializer p_serializer) override;

		/**
		* Defines how the behaviour should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;

	public:
		static LittleEngine::Eventing::Event<Behaviour*> CreatedEvent;
		static LittleEngine::Eventing::Event<Behaviour*> DestroyedEvent;

		const std::string name;

	private:
		sol::table m_object = sol::nil;
	};
}

#include "Modules/Framework/ECS/Behaviour.inl"
