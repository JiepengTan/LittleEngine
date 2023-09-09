/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "bullet/btBulletCollisionCommon.h"

#include "Core/Maths/FTransform.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FQuaternion.h"

namespace LittleEngine::Physics::Tools
{
	/**
	* Conversion helper to convert bullet maths to LittleEngine
	*/
	class Conversion
	{
	public:
		Conversion() = delete;

		/**
		* Convert a FTransform to btTransform
		* @param p_transform
		*/
		static btTransform ToBtTransform(const LittleEngine::FTransform& p_transform);

		/**
		* Convert a FVector3 to btVector3
		* @param p_vector
		*/
		static btVector3 ToBtVector3(const LittleEngine::FVector3& p_vector);

		/**
		* Convert a FQuaternion to btQuaternion
		* @param p_quaternion
		*/
		static btQuaternion ToBtQuaternion(const LittleEngine::FQuaternion& p_quaternion);

		/**
		* Convert a btTransform to FTransform
		* @param p_transform
		*/
		static LittleEngine::FTransform ToOvTransform(const btTransform& p_transform);

		/**
		* Convert a btVector3 to FVector3
		* @param p_vector
		*/
		static LittleEngine::FVector3 ToOvVector3(const btVector3& p_vector);

		/**
		* Convert a btQuaternion to FQuaternion
		* @param p_quaternion
		*/
		static LittleEngine::FQuaternion ToOvQuaternion(const btQuaternion& p_quaternion);
	};
}
