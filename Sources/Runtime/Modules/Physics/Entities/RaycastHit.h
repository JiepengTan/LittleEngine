/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <bullet/btBulletCollisionCommon.h>

#include <vector>


#include "Modules/Physics/Entities/PhysicalObject.h"

namespace LittleEngine::Physics::Entities
{
	/**
	* Data structure that holds raycast hit information
	*/
	struct RaycastHit
	{
		Entities::PhysicalObject* FirstResultObject = nullptr;
		std::vector<Entities::PhysicalObject*> ResultObjects;
	};
}
