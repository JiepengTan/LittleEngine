/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Editor/Core/EditorActions.h"

namespace LittleEditor::Core
{
	template<typename T>
	inline LittleEngine::Actor & EditorActions::CreateMonoComponentActor(bool p_focusOnCreation, LittleEngine::Actor* p_parent)
	{
		auto& instance = CreateEmptyActor(false, p_parent);

		T& component = instance.AddComponent<T>();

        instance.SetName(component.GetName());

		if (p_focusOnCreation)
			SelectActor(instance);

		return instance;
	}
}
