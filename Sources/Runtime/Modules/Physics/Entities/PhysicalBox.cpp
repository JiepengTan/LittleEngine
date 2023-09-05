/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Physics/Entities/PhysicalBox.h"
#include "Modules/Physics/Tools/Conversion.h"

LittleEngine::Physics::Entities::PhysicalBox::PhysicalBox(const LittleEngine::FVector3& p_size) : PhysicalObject()
{
	CreateCollisionShape(p_size);
	Init();
}

LittleEngine::Physics::Entities::PhysicalBox::PhysicalBox(LittleEngine::FTransform & p_transform, const LittleEngine::FVector3& p_size) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_size);
	Init();
}

void LittleEngine::Physics::Entities::PhysicalBox::SetSize(const LittleEngine::FVector3& p_size)
{
	if (m_size != p_size)
		RecreateCollisionShape(p_size);
}

LittleEngine::FVector3 LittleEngine::Physics::Entities::PhysicalBox::GetSize() const
{
	return m_size;
}


void LittleEngine::Physics::Entities::PhysicalBox::CreateCollisionShape(const LittleEngine::FVector3& p_size)
{
	m_shape = std::make_unique<btBoxShape>(LittleEngine::Physics::Tools::Conversion::ToBtVector3(p_size));
	m_size = p_size;
}

void LittleEngine::Physics::Entities::PhysicalBox::RecreateCollisionShape(const LittleEngine::FVector3& p_size)
{
	CreateCollisionShape(p_size);
	RecreateBody();
}

void LittleEngine::Physics::Entities::PhysicalBox::SetLocalScaling(const LittleEngine::FVector3 & p_scaling)
{
	m_shape->setLocalScaling(LittleEngine::Physics::Tools::Conversion::ToBtVector3(p_scaling));
}
