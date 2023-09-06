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

void LittleEngine::CPhysicalSphere::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnSerialize(p_doc, p_node);

	Serializer::SerializeFloat(p_doc, p_node, "radius", GetRadius());
}

void LittleEngine::CPhysicalSphere::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CPhysicalObject::OnDeserialize(p_doc, p_node);

	SetRadius(Serializer::DeserializeFloat(p_doc, p_node, "radius"));
}

void LittleEngine::CPhysicalSphere::OnInspector(LittleEngine::UI::Internal::WidgetContainer & p_root)
{
	CPhysicalObject::OnInspector(p_root);

	Helpers::GUIDrawer::DrawScalar<float>(p_root, "Radius", std::bind(&CPhysicalSphere::GetRadius, this), std::bind(&CPhysicalSphere::SetRadius, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
