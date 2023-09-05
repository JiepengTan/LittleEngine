/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/CoreInclude.h"

#include "Modules/Scripting/LuaBinder.h"
#include "Modules/Scripting/ScriptInterpreter.h"

LittleEngine::Scripting::ScriptInterpreter::ScriptInterpreter(const std::string& p_scriptRootFolder) :
	m_scriptRootFolder(p_scriptRootFolder)
{
	CreateLuaContextAndBindGlobals();

	/* Listen to behaviours */
	LittleEngine::Behaviour::CreatedEvent	+= std::bind(&ScriptInterpreter::Consider, this, std::placeholders::_1);
	LittleEngine::Behaviour::DestroyedEvent	+= std::bind(&ScriptInterpreter::Unconsider, this, std::placeholders::_1);
}

LittleEngine::Scripting::ScriptInterpreter::~ScriptInterpreter()
{
	DestroyLuaContext();
}

void LittleEngine::Scripting::ScriptInterpreter::CreateLuaContextAndBindGlobals()
{
	if (!m_luaState)
	{
		m_luaState = std::make_unique<sol::state>();
		m_luaState->open_libraries(sol::lib::base, sol::lib::math);
		LittleEngine::Scripting::LuaBinder::CallBinders(*m_luaState);
		m_isOk = true;

		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](Behaviour* behaviour)
		{
			if (!behaviour->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
				m_isOk = false;
		});

		if (!m_isOk)
			OVLOG_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
	}
}

void LittleEngine::Scripting::ScriptInterpreter::DestroyLuaContext()
{
	if (m_luaState)
	{
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](Behaviour* behaviour)
		{
			behaviour->UnregisterFromLuaContext();
		});

		m_luaState.reset();
		m_isOk = false;
	}
}

void LittleEngine::Scripting::ScriptInterpreter::Consider(LittleEngine::Behaviour* p_toConsider)
{
	if (m_luaState)
	{
		m_behaviours.push_back(p_toConsider);

		if (!p_toConsider->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
			m_isOk = false;
	}
}

void LittleEngine::Scripting::ScriptInterpreter::Unconsider(LittleEngine::Behaviour* p_toUnconsider)
{
	if (m_luaState)
		p_toUnconsider->UnregisterFromLuaContext();

	m_behaviours.erase(std::remove_if(m_behaviours.begin(), m_behaviours.end(), [p_toUnconsider](Behaviour* behaviour)
	{
		return p_toUnconsider == behaviour;
	}));

	RefreshAll(); // Unconsidering a script is impossible with Lua, we have to reparse every behaviours
}

void LittleEngine::Scripting::ScriptInterpreter::RefreshAll()
{
	DestroyLuaContext();
	CreateLuaContextAndBindGlobals();
}

bool LittleEngine::Scripting::ScriptInterpreter::IsOk() const
{
	return m_isOk;
}
