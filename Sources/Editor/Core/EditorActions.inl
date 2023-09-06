/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Editor/Core/EditorActions.h"

namespace LittleEngine::Editor::Core
{
	template<typename T>
	inline ActorPtr EditorActions::CreateMonoComponentActor(bool p_focusOnCreation, ActorPtr p_parent)
	{
		auto instance = CreateEmptyActor(false, p_parent);

		SharedPtr<T> component = instance->AddComponent<T>();

        instance->SetName(component->GetName());

		if (p_focusOnCreation)
			SelectActor(instance);

		return instance;
	}
}
