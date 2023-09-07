/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Physics/Entities/PhysicalCapsule.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"

#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Actor.h"

using namespace LittleEngine::Physics::Entities;



void LittleEngine::CPhysicalCapsule::DoInit(ActorPtr p_owner)
{
	CPhysicalObject::DoInit(p_owner);
	m_physicalObject = std::make_unique<PhysicalCapsule>(p_owner->transform->GetFTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string LittleEngine::CPhysicalCapsule::GetName()
{
	return "Physical Capsule";
}

void LittleEngine::CPhysicalCapsule::SetRadius(float p_radius)
{
	GetPhysicalObjectAs<PhysicalCapsule>().SetRadius(p_radius);
}

void LittleEngine::CPhysicalCapsule::SetHeight(float p_height)
{
	GetPhysicalObjectAs<PhysicalCapsule>().SetHeight(p_height);
}

float LittleEngine::CPhysicalCapsule::GetRadius() const
{
	return GetPhysicalObjectAs<PhysicalCapsule>().GetRadius();
}

float LittleEngine::CPhysicalCapsule::GetHeight() const
{
	return GetPhysicalObjectAs<PhysicalCapsule>().GetHeight();
}

void LittleEngine::CPhysicalCapsule::OnSerialize(ISerializer p_serializer)
{
	CPhysicalObject::OnSerialize(p_serializer);

	SerializeUtil::SerializeFloat("radius", GetRadius());
	SerializeUtil::SerializeFloat("height", GetHeight());
}

void LittleEngine::CPhysicalCapsule::OnDeserialize(ISerializer p_serializer)
{
	CPhysicalObject::OnDeserialize(p_serializer);

	SetRadius(SerializeUtil::DeserializeFloat("radius"));
	SetHeight(SerializeUtil::DeserializeFloat("height"));
}

void LittleEngine::CPhysicalCapsule::OnInspector()
{
	CPhysicalObject::OnInspector();

	GUIUtil::DrawScalar<float>( "Radius", std::bind(&CPhysicalCapsule::GetRadius, this), std::bind(&CPhysicalCapsule::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
	GUIUtil::DrawScalar<float>( "Height", std::bind(&CPhysicalCapsule::GetHeight, this), std::bind(&CPhysicalCapsule::SetHeight, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
