/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Core/CoreInclude.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/Behaviour.h"
#include "Modules/Scripting/LuaBinder.h"

LittleEngine::Eventing::Event<LittleEngine::Behaviour*> LittleEngine::Behaviour::CreatedEvent;
LittleEngine::Eventing::Event<LittleEngine::Behaviour*> LittleEngine::Behaviour::DestroyedEvent;

LittleEngine::Behaviour::Behaviour(Actor& p_owner, const std::string& p_name) :
	name(p_name), AComponent(p_owner)
{
	CreatedEvent.Invoke(this);
}

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
	using namespace LittleEngine::Scripting;

	auto result = p_luaState.safe_script_file(p_scriptFolder + name + ".lua", &sol::script_pass_on_error);

	if (!result.valid())
	{
		sol::error err = result;
		OVLOG_ERROR(err.what());
		return false;
	}
	else
	{
		if (result.return_count() == 1 && result[0].is<sol::table>())
		{
			m_object = result[0];
			m_object["owner"] = owner;
			return true;
		}
		else
		{
			OVLOG_ERROR("'" + name + ".lua' missing return expression");
			return false;
		}
	}
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

void LittleEngine::Behaviour::OnCollisionEnter(CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStart", p_otherObject); // Retro-compatibility
	LuaCall("OnCollisionEnter", p_otherObject);
}

void LittleEngine::Behaviour::OnCollisionStay(CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStay", p_otherObject);
}

void LittleEngine::Behaviour::OnCollisionExit(CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStop", p_otherObject); // Retro-compatibility
	LuaCall("OnCollisionExit", p_otherObject);
}

void LittleEngine::Behaviour::OnTriggerEnter(CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStart", p_otherObject); // Retro-compatibility
	LuaCall("OnTriggerEnter", p_otherObject);
}

void LittleEngine::Behaviour::OnTriggerStay(CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStay", p_otherObject);
}

void LittleEngine::Behaviour::OnTriggerExit(CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStop", p_otherObject); // Retro-compatibility
	LuaCall("OnTriggerExit", p_otherObject);
}

void LittleEngine::Behaviour::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
}

void LittleEngine::Behaviour::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
}

void LittleEngine::Behaviour::OnInspector(LittleEngine::UI::Internal::WidgetContainer & p_root)
{
	using namespace LittleEngine;
	using namespace LittleEngine::Helpers;

	if (m_object.valid())
	{
		p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Ready", LittleEngine::UI::Types::Color::Green);
		p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Your script gets interpreted by the engine with success", LittleEngine::UI::Types::Color::White);
	}
	else
	{
		p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Compilation failed!", LittleEngine::UI::Types::Color::Red);
		p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Check the console for more information", LittleEngine::UI::Types::Color::White);
	}
}
