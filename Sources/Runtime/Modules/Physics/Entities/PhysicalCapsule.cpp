/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "Modules/Physics/Entities/PhysicalCapsule.h"
#include "Modules/Physics/Tools/Conversion.h"

LittleEngine::Physics::Entities::PhysicalCapsule::PhysicalCapsule(float p_radius, float p_height) : PhysicalObject()
{
	CreateCollisionShape(p_radius, p_height);
	Init();
}

LittleEngine::Physics::Entities::PhysicalCapsule::PhysicalCapsule(LittleEngine::FTransform & p_transform, float p_radius, float p_height) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_radius, p_height);
	Init();
}

void LittleEngine::Physics::Entities::PhysicalCapsule::SetRadius(float p_radius)
{
	RecreateCollisionShape(p_radius, m_height);
}

void LittleEngine::Physics::Entities::PhysicalCapsule::SetHeight(float p_height)
{
	RecreateCollisionShape(m_radius, p_height);
}

float LittleEngine::Physics::Entities::PhysicalCapsule::GetRadius() const
{
	return m_radius;
}

float LittleEngine::Physics::Entities::PhysicalCapsule::GetHeight() const
{
	return m_height;
}

void LittleEngine::Physics::Entities::PhysicalCapsule::CreateCollisionShape(float p_radius, float p_height)
{
	m_shape = std::make_unique<btCapsuleShape>(p_radius, p_height);
	m_radius = p_radius;
	m_height = p_height;
}

void LittleEngine::Physics::Entities::PhysicalCapsule::RecreateCollisionShape(float p_radius, float p_height)
{
	CreateCollisionShape(p_radius, p_height);
	RecreateBody();
}

void LittleEngine::Physics::Entities::PhysicalCapsule::SetLocalScaling(const LittleEngine::FVector3 & p_scaling)
{
	m_shape->setLocalScaling({ std::max(p_scaling.x, p_scaling.z), p_scaling.y, 1.0f });
}
