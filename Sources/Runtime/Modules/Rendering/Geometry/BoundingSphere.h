/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/FVector3.h"

namespace LittleEngine::Rendering::Geometry
{
	/**
	* Data structure that defines a bounding sphere (Position + radius)
	*/
	REFLECTION_TYPE(BoundingSphere)
	STRUCT (BoundingSphere , Fields)
	{
		REFLECTION_BODY(BoundingSphere)
	public:
		LittleEngine::FVector3 position;
		float radius;
	};
}
