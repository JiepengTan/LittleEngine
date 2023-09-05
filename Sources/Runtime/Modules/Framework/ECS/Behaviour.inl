/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "Core/CoreInclude.h"
#include "Modules/Framework/ECS/Behaviour.h"

namespace LittleEngine
{
	template<typename ...Args>
	inline void Behaviour::LuaCall(const std::string& p_functionName, Args&& ...p_args)
	{
		if (m_object.valid())
		{
			if (m_object[p_functionName].valid())
			{
				sol::protected_function pfr = m_object[p_functionName];
				auto pfrResult = pfr.call(m_object, std::forward<Args>(p_args)...);
				if (!pfrResult.valid())
				{
					sol::error err = pfrResult;
					OVLOG_ERROR(err.what());
				}
			}
		}
	}
}
