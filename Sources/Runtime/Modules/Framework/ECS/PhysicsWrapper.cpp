/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/PhysicsWrapper.h"

#include "Modules/Framework/Global/ServiceLocator.h"

#include "Modules/Physics/Core/PhysicsEngine.h"

std::optional<LittleEngine::PhysicsWrapper::RaycastHit> LittleEngine::PhysicsWrapper::Raycast(LittleEngine::FVector3 p_origin, LittleEngine::FVector3 p_direction, float p_distance)
{
	if (auto result = OVSERVICE(LittleEngine::Physics::Core::PhysicsEngine).Raycast(p_origin, p_direction, p_distance))
	{
		RaycastHit finalResult;

		finalResult.FirstResultObject = std::addressof(result.value().FirstResultObject->GetUserData<std::reference_wrapper<CPhysicalObject>>().get());
		for (auto object : result.value().ResultObjects)
			finalResult.ResultObjects.push_back(std::addressof(object->GetUserData<std::reference_wrapper<CPhysicalObject>>().get()));

		return finalResult;
	}
	else
		return {};
}
