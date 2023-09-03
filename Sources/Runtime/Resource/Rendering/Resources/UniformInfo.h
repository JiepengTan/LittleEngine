/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>
#include <any>

#include "Resource/Rendering/Resources/UniformType.h"

namespace OvRendering::Resources
{
	/**
	* Data structure containing information about a uniform
	*/
	struct UniformInfo
	{
		UniformType		type;
		std::string		name;
		uint32_t		location;
		std::any		defaultValue;
	};
}
