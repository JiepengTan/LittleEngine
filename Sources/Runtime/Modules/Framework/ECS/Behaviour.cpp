/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Core/CoreInclude.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Behaviour.h"
#include "Modules/Scripting/LuaBinder.h"

LittleEngine::Eventing::Event<LittleEngine::Behaviour*> LittleEngine::Behaviour::CreatedEvent;
LittleEngine::Eventing::Event<LittleEngine::Behaviour*> LittleEngine::Behaviour::DestroyedEvent;


LittleEngine::Behaviour::~Behaviour()
{
	DestroyedEvent.Invoke(this);
}

std::string LittleEngine::Behaviour::GetName()
{
	return "Behaviour";
}

bool LittleEngine::Behaviour::RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder)
{
	return true;
}

void LittleEngine::Behaviour::UnregisterFromLuaContext()
{
	m_object = sol::nil;
}

sol::table& LittleEngine::Behaviour::GetTable()
{
	return m_object;
}

void LittleEngine::Behaviour::OnAwake()
{
	LuaCall("OnAwake");
}

void LittleEngine::Behaviour::OnStart()
{
	LuaCall("OnStart");
}

void LittleEngine::Behaviour::OnEnable()
{
	LuaCall("OnEnable");
}

void LittleEngine::Behaviour::OnDisable()
{
	LuaCall("OnDisable");
}

void LittleEngine::Behaviour::OnDestroy()
{
	LuaCall("OnEnd"); // Retro-compatibility
	LuaCall("OnDestroy");
}

void LittleEngine::Behaviour::OnUpdate(float p_deltaTime)
{
	LuaCall("OnUpdate", p_deltaTime);
}

void LittleEngine::Behaviour::OnFixedUpdate(float p_deltaTime)
{
	LuaCall("OnFixedUpdate", p_deltaTime);
}

void LittleEngine::Behaviour::OnLateUpdate(float p_deltaTime)
{
	LuaCall("OnLateUpdate", p_deltaTime);
}

void LittleEngine::Behaviour::OnCollisionEnter(SharedPtr<CPhysicalObject> p_otherObject)
{
	LuaCall("OnCollisionStart", p_otherObject); // Retro-compatibility
	LuaCall("OnCollisionEnter", p_otherObject);
}

void LittleEngine::Behaviour::OnCollisionStay(SharedPtr<CPhysicalObject> p_otherObject)
{
	LuaCall("OnCollisionStay", p_otherObject);
}

void LittleEngine::Behaviour::OnCollisionExit(SharedPtr<CPhysicalObject> p_otherObject)
{
	LuaCall("OnCollisionStop", p_otherObject); // Retro-compatibility
	LuaCall("OnCollisionExit", p_otherObject);
}

void LittleEngine::Behaviour::OnTriggerEnter(SharedPtr<CPhysicalObject> p_otherObject)
{
	LuaCall("OnTriggerStart", p_otherObject); // Retro-compatibility
	LuaCall("OnTriggerEnter", p_otherObject);
}

void LittleEngine::Behaviour::OnTriggerStay(SharedPtr<CPhysicalObject> p_otherObject)
{
	LuaCall("OnTriggerStay", p_otherObject);
}

void LittleEngine::Behaviour::OnTriggerExit(SharedPtr<CPhysicalObject> p_otherObject)
{
	LuaCall("OnTriggerStop", p_otherObject); // Retro-compatibility
	LuaCall("OnTriggerExit", p_otherObject);
}

void LittleEngine::Behaviour::OnSerialize(ISerializer p_serializer)
{
}

void LittleEngine::Behaviour::OnDeserialize(ISerializer p_serializer)
{
}

void LittleEngine::Behaviour::OnInspector()
{
	using namespace LittleEngine;
	

	if (m_object.valid())
	{
		GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Ready", LittleEngine::Color::Green);
		GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Your script gets interpreted by the engine with success", LittleEngine::Color::White);
	}
	else
	{
		GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Compilation failed!", LittleEngine::Color::Red);
		GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Check the console for more information", LittleEngine::Color::White);
	}
}
