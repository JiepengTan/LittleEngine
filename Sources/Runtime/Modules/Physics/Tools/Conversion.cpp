/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Physics/Tools/Conversion.h"

btTransform LittleEngine::Physics::Tools::Conversion::ToBtTransform(const LittleEngine::FTransform& p_transform)
{
	return btTransform(ToBtQuaternion(p_transform.GetWorldRotation()), ToBtVector3(p_transform.GetWorldPosition()));
}

btVector3 LittleEngine::Physics::Tools::Conversion::ToBtVector3(const LittleEngine::FVector3& p_vector)
{
	return btVector3(p_vector.x, p_vector.y, p_vector.z);
}

btQuaternion LittleEngine::Physics::Tools::Conversion::ToBtQuaternion(const LittleEngine::FQuaternion& p_quaternion)
{
	return btQuaternion(p_quaternion.x, p_quaternion.y, p_quaternion.z, p_quaternion.w);
}

LittleEngine::FTransform LittleEngine::Physics::Tools::Conversion::ToOvTransform(const btTransform& p_transform)
{
	LittleEngine::FQuaternion q = ToOvQuaternion(p_transform.getRotation());
	LittleEngine::FVector3  v = ToOvVector3(p_transform.getOrigin());
	return LittleEngine::FTransform(v, q);
}

LittleEngine::FVector3 LittleEngine::Physics::Tools::Conversion::ToOvVector3(const btVector3& p_vector)
{
	return LittleEngine::FVector3(p_vector.getX(), p_vector.getY(), p_vector.getZ());
}

LittleEngine::FQuaternion LittleEngine::Physics::Tools::Conversion::ToOvQuaternion(const btQuaternion& p_quaternion)
{
	return LittleEngine::FQuaternion(p_quaternion.getX(), p_quaternion.getY(), p_quaternion.getZ(), p_quaternion.getW());
}
