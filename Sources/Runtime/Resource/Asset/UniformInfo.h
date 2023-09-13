/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"
#include "any"

#include "Resource/Asset/UniformType.h"
#include "Resource/Data/ResIncludeRendering.h"

namespace LittleEngine::Resources
{
	/**
	* Data structure containing information about a uniform
	*/
	REFLECTION_STRUCT_TYPE(UniformInfo)
	STRUCT(UniformInfo: public ResData, Fields)
	{
		REFLECTION_STRUCT_BODY(UniformInfo)
	public:
		UniformType		type;
		std::string		name;
		uint32_t		location;
		ResUniformInfo	defaultValue;
	};
}
