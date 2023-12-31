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
	REFLECTION_STRUCT_TYPE(BoundingSphere)
	STRUCT (BoundingSphere , Fields)
	{
		REFLECTION_STRUCT_BODY(BoundingSphere)
	public:
		BoundingSphere(float p_radius = 1.0f)
		{
			radius = p_radius;
			position = FVector3::Zero;
		}
		LittleEngine::FVector3 position;
		float radius;
	};
}
