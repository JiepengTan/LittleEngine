/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <memory>

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
			comp->DoInit(ptr);
			m_components[comp->GetTypeID()] = comp;
			NotifyComponentAdd(comp);
			if (m_playing && IsActive())
			{
				comp->OnAwake();
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
		auto key = T::GetTypeID();
		if(m_components.count(key) != 0)
		{
			auto comp = m_components.at(key);
			NotifyComponentRemoved(comp);
			m_components.erase(key);
			return true;
		}
		return false;
	}

	template<typename T>
	SharedPtr<T> Actor::GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T should derive from AComponent");
		auto key = T::GetTypeID();
		if(m_components.count(key) != 0)
		{
			Component* ptr = m_components.at(key).get();
			return SharedPtr<T>(reinterpret_cast<T*>(ptr));// TODO can cast shared_ptr??
		}
		return nullptr;
	}
}
