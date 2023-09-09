/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "optional"
#include "vector"

#include "Modules/Physics/Entities/RaycastHit.h"

#include "Core/Maths/FVector3.h"

#include "Modules/Framework/ECS/Components/CPhysicalObject.h"

namespace LittleEngine
{
	/**
	* Simple class that contains wrappers for LittleEngine::Physics in an ECS style
	*/
	class PhysicsWrapper
	{
	public:
		/**
		* Simple data structure that wraps the physics RaycastHit with physics components
		*/
		struct RaycastHit
		{
			CPhysicalObject* FirstResultObject = nullptr;
			std::vector<CPhysicalObject*> ResultObjects;
		};

		/* Casts a ray against all Physical Object in the Scene and returns information on what was hit
		 * @param p_origin
		 * @param p_end
		 */
		static std::optional<RaycastHit> Raycast(LittleEngine::FVector3 p_origin, LittleEngine::FVector3 p_direction, float p_distance);
	};
}
