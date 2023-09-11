/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "algorithm"

#include "Core/Tools/Filesystem/PathUtil.h"
#include "Core/Tools/Utils/StringUtil.h"
#include "Resource/Manager/AResourceManager.h"

namespace LittleEngine::ResourceManagement
{
	template<typename T>
	inline T* AResourceManager<T>::LoadResource(const std::string & p_path)
	{
		auto path = StringUtil::Replace(p_path,"\\","/");
		if (auto resource = GetResource(path, false); resource)
			return resource;
		else
		{
			auto newResource = CreateResource(path);
			if (newResource)
				return RegisterResource(path, newResource);
			else
				return nullptr;
		}
	}

	template<typename T>
	inline void AResourceManager<T>::UnloadResource(const std::string & p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
		{
			DestroyResource(resource);
			UnregisterResource(p_path);
		}
	}

	template<typename T>
	inline bool AResourceManager<T>::MoveResource(const std::string & p_previousPath, const std::string & p_newPath)
	{
		if (IsResourceRegistered(p_previousPath) && !IsResourceRegistered(p_newPath))
		{
			T* toMove = m_resources.at(p_previousPath);
			UnregisterResource(p_previousPath);
			RegisterResource(p_newPath, toMove);
			return true;
		}

		return false;
	}

	template<typename T>
	inline void AResourceManager<T>::ReloadResource(const std::string& p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
		{
			ReloadResource(resource, p_path);
		}
	}

	template<typename T>
	inline bool AResourceManager<T>::IsResourceRegistered(const std::string & p_path)
	{
		return m_resources.find(p_path) != m_resources.end();
	}

	template<typename T>
	inline void AResourceManager<T>::UnloadResources()
	{
		for (auto&[key, value] : m_resources)
			DestroyResource(value);

		m_resources.clear();
	}

	template<typename T>
	inline T* AResourceManager<T>::RegisterResource(const std::string& p_path, T* p_instance)
	{
		if (auto resource = GetResource(p_path, false); resource)
			DestroyResource(resource);

		auto path = StringUtil::Replace(p_path,"\\","/");
		m_resources[path] = p_instance;

		return p_instance;
	}

	template<typename T>
	inline void AResourceManager<T>::UnregisterResource(const std::string & p_path)
	{
		auto path = StringUtil::Replace(p_path,"\\","/");
		m_resources.erase(path);
	}

	template<typename T>
	inline T* AResourceManager<T>::GetResource(const std::string& p_path, bool p_tryToLoadIfNotFound)
	{
		if(p_path.empty()) return nullptr;
		auto path = StringUtil::Replace(p_path,"\\","/");
		if (auto resource = m_resources.find(path); resource != m_resources.end())
		{
			return resource->second;
		}
		else if (p_tryToLoadIfNotFound)
		{
			return LoadResource(path);
		}

		return nullptr;
	}

	template<typename T>
	inline T* AResourceManager<T>::operator[](const std::string & p_path)
	{
		return GetResource(p_path);
	}

	template<typename T>
	inline void AResourceManager<T>::ProvideAssetPaths(const std::string & p_projectAssetsPath, const std::string & p_engineAssetsPath)
	{
		__PROJECT_ASSETS_PATH	= p_projectAssetsPath;
		__ENGINE_ASSETS_PATH	= p_engineAssetsPath;
	}

	template<typename T>
	inline std::unordered_map<std::string, T*>& AResourceManager<T>::GetResources()
	{
		return m_resources;
	}

}
