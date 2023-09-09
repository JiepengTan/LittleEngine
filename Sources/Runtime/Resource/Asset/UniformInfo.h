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
	struct UniformInfo
	{
		UniformType		type;
		std::string		name;
		uint32_t		location;
		ResUniformInfo	defaultValue;
	};
}
