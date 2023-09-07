/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Physics/Entities/PhysicalSphere.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"

#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Actor.h"

using namespace LittleEngine::Physics::Entities;

void LittleEngine::CPhysicalSphere::DoInit(ActorPtr p_owner)
{
	CPhysicalObject::DoInit(p_owner);
	m_physicalObject = std::make_unique<PhysicalSphere>(p_owner->transform->GetFTransform());

	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string LittleEngine::CPhysicalSphere::GetName()
{
	return "Physical Sphere";
}

void LittleEngine::CPhysicalSphere::SetRadius(float p_radius)
{
	GetPhysicalObjectAs<PhysicalSphere>().SetRadius(p_radius);
}

float LittleEngine::CPhysicalSphere::GetRadius() const
{
	return GetPhysicalObjectAs<PhysicalSphere>().GetRadius();
}

void LittleEngine::CPhysicalSphere::OnSerialize(ISerializer p_serializer)
{
	CPhysicalObject::OnSerialize(p_serializer);

	SerializeUtil::SerializeFloat("radius", GetRadius());
}

void LittleEngine::CPhysicalSphere::OnDeserialize(ISerializer p_serializer)
{
	CPhysicalObject::OnDeserialize(p_serializer);

	SetRadius(SerializeUtil::DeserializeFloat("radius"));
}

void LittleEngine::CPhysicalSphere::OnInspector()
{
	CPhysicalObject::OnInspector();

	GUIUtil::DrawScalar<float>( "Radius", std::bind(&CPhysicalSphere::GetRadius, this), std::bind(&CPhysicalSphere::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
