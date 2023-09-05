/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/FVector3.h"

namespace LittleEngine::Physics::Settings
{
	/**
	* Data structure to give to the PhysicsEngine constructor to setup its settings
	*/
	struct PhysicsSettings
	{
		LittleEngine::FVector3 gravity = { 0.0f, -9.81f, 0.f };
	};
}
