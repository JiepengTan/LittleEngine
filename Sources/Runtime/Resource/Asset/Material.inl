/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/CoreInclude.h"

#include "Resource/Asset/Material.h"

namespace LittleEngine
{
	template <typename T>
	void Material::SetProperty(const std::string p_key, const T& p_value, bool p_isForce)
	{
		auto ptr= GetProperty(p_key);
		if(ptr == nullptr)
		{
			ResUniformInfo newVal;
			newVal.m_key = p_key;
			newVal.Set(p_value);
			m_res.m_uniformsData.push_back(newVal);
		}else
		{
			ptr->Set(p_value);
		}
	}

	template <typename T>
	void Material::GetProperty(const std::string p_key, T& val)
	{
		auto ptr= GetProperty(p_key);
		if(ptr != nullptr)
			ptr->Get(val);
	}
}
