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
			auto basePtr = std::dynamic_pointer_cast<Component>(comp);
			OnAddComponent(basePtr);
			return std::dynamic_pointer_cast<T>(basePtr);
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
