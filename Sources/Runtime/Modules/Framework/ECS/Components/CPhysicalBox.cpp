/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Physics/Entities/PhysicalBox.h"

#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Actor.h"

using namespace LittleEngine::Physics::Entities;


void LittleEngine::CPhysicalBox::OnAwake()
{
	CPhysicalObject::OnAwake();
	m_physicalObject = std::make_unique<LittleEngine::Physics::Entities::PhysicalBox>
			(GetActor()->transform->GetFTransform());
	m_physicalObject->SetUserData<std::reference_wrapper<CPhysicalObject>>(*this);

	BindListener();
	Init();
}

std::string LittleEngine::CPhysicalBox::GetName()
{
	return "Physical Box";
}

void LittleEngine::CPhysicalBox::SetSize(const LittleEngine::FVector3 & p_size)
{
	GetPhysicalObjectAs<PhysicalBox>().SetSize(p_size);
}

LittleEngine::FVector3 LittleEngine::CPhysicalBox::GetSize() const
{
	return GetPhysicalObjectAs<PhysicalBox>().GetSize();
}

void LittleEngine::CPhysicalBox::OnSerialize(ISerializer p_serializer)
{
	CPhysicalObject::OnSerialize(p_serializer);

	SerializeUtil::SerializeVec3("size", GetSize());
}

void LittleEngine::CPhysicalBox::OnDeserialize(ISerializer p_serializer)
{
	CPhysicalObject::OnDeserialize(p_serializer);

	SetSize(SerializeUtil::DeserializeVec3("size"));
}

void LittleEngine::CPhysicalBox::OnInspector()
{
	CPhysicalObject::OnInspector();

	GUIUtil::DrawVec3( "Size", std::bind(&CPhysicalBox::GetSize, this), std::bind(&CPhysicalBox::SetSize, this, std::placeholders::_1), 0.1f, 0.f, 100000.f);
}
