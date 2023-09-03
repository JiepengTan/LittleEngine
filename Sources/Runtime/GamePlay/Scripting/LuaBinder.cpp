/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "GamePlay/Scripting/LuaBinder.h"

#include "GamePlay/Scripting/LuaMathsBinder.h"
#include "GamePlay/Scripting/LuaActorBinder.h"
#include "GamePlay/Scripting/LuaComponentBinder.h"
#include "GamePlay/Scripting/LuaGlobalsBinder.h"

void OvCore::Scripting::LuaBinder::CallBinders(sol::state& p_luaState)
{
	auto& L = p_luaState;

	LuaMathsBinder::BindMaths(L);
	LuaActorBinder::BindActor(L);
	LuaComponentBinder::BindComponent(L);
	LuaGlobalsBinder::BindGlobals(L);
}
