/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "memory"

#include "Modules/Framework/ECS/Actor.h"

namespace LittleEngine
{
	template<typename T>
	SharedPtr<T> Actor::AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T should derive from AComponent");
		if (auto found = GetComponent<T>(); !found)
		{
			auto comp = std::make_shared<T>();
			ActorPtr ptr = GetSceneActor(m_actorID);
			LE_ASSERT(ptr!= nullptr,"Can not find a actor in scene" + std::to_string(m_actorID));
			comp->SetActor(ptr);
			m_components.push_back(comp);
			comp->OnAwake();
			NotifyComponentAdd(comp);
			if (m_playing && IsActive())
			{
				comp->OnEnable();
				comp->OnStart();
			}		
			return comp;
		}
		else
		{
			return found;
		}
	}

	template<typename T>
	bool Actor::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T should derive from AComponent");
		static_assert(!std::is_same<CTransform, T>::value, "You can't remove a CTransform from an actor");
		auto key = T::GetStaticTypeID();
		for (auto it = m_components.begin();it!= m_components.end();++it)
		{
			auto& item = *it;
			if (item->IsAssignableFrom(key))
			{
				NotifyComponentRemoved(item);
				m_components.erase(it);
				return true;
			}
		}
		
		return false;
	}

	template<typename T>
	SharedPtr<T> Actor::GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T should derive from AComponent");
		auto key = T::GetStaticTypeID();
		for (auto& item : m_components)
		{
			if(item->IsAssignableFrom(key))
				return std::dynamic_pointer_cast<T>(item);
		}
		return nullptr;
	}
}
