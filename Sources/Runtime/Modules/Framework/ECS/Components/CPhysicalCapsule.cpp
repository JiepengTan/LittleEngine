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

void LittleEngine::CPhysicalCapsule::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Serializer::SerializeFloat(p_doc, p_node, "radius", GetRadius());
	Serializer::SerializeFloat(p_doc, p_node, "height", GetHeight());
}

void LittleEngine::CPhysicalCapsule::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetRadius(Serializer::DeserializeFloat(p_doc, p_node, "radius"));
	SetHeight(Serializer::DeserializeFloat(p_doc, p_node, "height"));
}

void LittleEngine::CPhysicalCapsule::OnInspector(LittleEngine::UI::Internal::WidgetContainer & p_root)
{
	CPhysicalObject::OnInspector(p_root);

	Helpers::GUIDrawer::DrawScalar<float>(p_root, "Radius", std::bind(&CPhysicalCapsule::GetRadius, this), std::bind(&CPhysicalCapsule::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
	Helpers::GUIDrawer::DrawScalar<float>(p_root, "Height", std::bind(&CPhysicalCapsule::GetHeight, this), std::bind(&CPhysicalCapsule::SetHeight, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
