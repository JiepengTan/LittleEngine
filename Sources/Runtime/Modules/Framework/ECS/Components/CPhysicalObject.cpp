/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Selection/ComboBox.h"

#include "Core/CoreInclude.h"

#include "Modules/Physics/Entities/PhysicalObject.h"

#include "Modules/Framework/ECS/Components/CPhysicalObject.h"
#include "Modules/Framework/ECS/Actor.h"

LittleEngine::CPhysicalObject::CPhysicalObject(Actor & p_owner) : 
	Component(p_owner)
{
}

void LittleEngine::CPhysicalObject::AddForce(const LittleEngine::FVector3 & p_force)
{
	m_physicalObject->AddForce(p_force);
}

void LittleEngine::CPhysicalObject::AddImpulse(const LittleEngine::FVector3 & p_impulse)
{
	m_physicalObject->AddImpulse(p_impulse);
}

void LittleEngine::CPhysicalObject::ClearForces()
{
	m_physicalObject->ClearForces();
}

void LittleEngine::CPhysicalObject::SetCollisionDetectionMode(LittleEngine::Physics::Entities::PhysicalObject::ECollisionDetectionMode p_mode)
{
	m_physicalObject->SetCollisionDetectionMode(p_mode);
}

LittleEngine::Physics::Entities::PhysicalObject::ECollisionDetectionMode LittleEngine::CPhysicalObject::GetCollisionDetectionMode() const
{
	return m_physicalObject->GetCollisionDetectionMode();
}

float LittleEngine::CPhysicalObject::GetMass() const
{
	return m_physicalObject->GetMass();
}

float LittleEngine::CPhysicalObject::GetBounciness() const
{
	return m_physicalObject->GetBounciness();
}

float LittleEngine::CPhysicalObject::GetFriction() const
{
	return m_physicalObject->GetFriction();
}

LittleEngine::FVector3 LittleEngine::CPhysicalObject::GetLinearVelocity() const
{
	return m_physicalObject->GetLinearVelocity();
}

LittleEngine::FVector3 LittleEngine::CPhysicalObject::GetAngularVelocity() const
{
	return m_physicalObject->GetAngularVelocity();
}

LittleEngine::FVector3 LittleEngine::CPhysicalObject::GetLinearFactor() const
{
	return m_physicalObject->GetLinearFactor();
}

LittleEngine::FVector3 LittleEngine::CPhysicalObject::GetAngularFactor() const
{
	return m_physicalObject->GetAngularFactor();
}

bool LittleEngine::CPhysicalObject::IsTrigger() const
{
	return m_physicalObject->IsTrigger();
}

bool LittleEngine::CPhysicalObject::IsKinematic() const
{
	return m_physicalObject->IsKinematic();
}

LittleEngine::Physics::Entities::PhysicalObject::EActivationState LittleEngine::CPhysicalObject::GetActivationState() const
{
	return m_physicalObject->GetActivationState();
}

void LittleEngine::CPhysicalObject::SetMass(float p_mass)
{
	m_physicalObject->SetMass(p_mass);
}

void LittleEngine::CPhysicalObject::SetBounciness(float p_bounciness)
{
	m_physicalObject->SetBounciness(p_bounciness);
}

void LittleEngine::CPhysicalObject::SetFriction(float p_friction)
{
	m_physicalObject->SetFriction(p_friction);
}

void LittleEngine::CPhysicalObject::SetLinearVelocity(const LittleEngine::FVector3 & p_linearVelocity)
{
	m_physicalObject->SetLinearVelocity(p_linearVelocity);
}

void LittleEngine::CPhysicalObject::SetAngularVelocity(const LittleEngine::FVector3 & p_angularVelocity)
{
	m_physicalObject->SetAngularVelocity(p_angularVelocity);
}

void LittleEngine::CPhysicalObject::SetLinearFactor(const LittleEngine::FVector3 & p_linearFactor)
{
	m_physicalObject->SetLinearFactor(p_linearFactor);
}

void LittleEngine::CPhysicalObject::SetAngularFactor(const LittleEngine::FVector3 & p_angularFactor)
{
	m_physicalObject->SetAngularFactor(p_angularFactor);
}

void LittleEngine::CPhysicalObject::SetTrigger(bool p_trigger)
{
	m_physicalObject->SetTrigger(p_trigger);
}

void LittleEngine::CPhysicalObject::SetKinematic(bool p_kinematic)
{
	m_physicalObject->SetKinematic(p_kinematic);
}

void LittleEngine::CPhysicalObject::SetActivationState(LittleEngine::Physics::Entities::PhysicalObject::EActivationState p_state)
{
	m_physicalObject->SetActivationState(p_state);
}

void LittleEngine::CPhysicalObject::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	Serializer::SerializeBoolean(p_doc, p_node, "is_trigger", IsTrigger());
	Serializer::SerializeBoolean(p_doc, p_node, "is_kinematic", IsKinematic());
	Serializer::SerializeFloat(p_doc, p_node, "bounciness", GetBounciness());
	Serializer::SerializeFloat(p_doc, p_node, "mass", GetMass());
	Serializer::SerializeFloat(p_doc, p_node, "friction", GetFriction());
	Serializer::SerializeVec3(p_doc, p_node, "linear_factor", GetLinearFactor());
	Serializer::SerializeVec3(p_doc, p_node, "angular_factor", GetAngularFactor());
	Serializer::SerializeInt(p_doc, p_node, "collision_mode", static_cast<int>(GetCollisionDetectionMode()));
}

void LittleEngine::CPhysicalObject::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	SetTrigger(Serializer::DeserializeBoolean(p_doc, p_node, "is_trigger"));
	SetKinematic(Serializer::DeserializeBoolean(p_doc, p_node, "is_kinematic"));
	SetBounciness(Serializer::DeserializeFloat(p_doc, p_node, "bounciness"));
	SetMass(Serializer::DeserializeFloat(p_doc, p_node, "mass"));
	SetFriction(Serializer::DeserializeFloat(p_doc, p_node, "friction"));
	SetLinearFactor(Serializer::DeserializeVec3(p_doc, p_node, "linear_factor"));
	SetAngularFactor(Serializer::DeserializeVec3(p_doc, p_node, "angular_factor"));
	SetCollisionDetectionMode(static_cast<LittleEngine::Physics::Entities::PhysicalObject::ECollisionDetectionMode>(Serializer::DeserializeInt(p_doc, p_node, "collision_mode")));
}

void LittleEngine::CPhysicalObject::OnInspector(LittleEngine::UI::Internal::WidgetContainer & p_root)
{
	Helpers::GUIDrawer::DrawBoolean(p_root, "Trigger", std::bind(&CPhysicalObject::IsTrigger, this), std::bind(&CPhysicalObject::SetTrigger, this, std::placeholders::_1));
	Helpers::GUIDrawer::DrawBoolean(p_root, "Kinematic", std::bind(&CPhysicalObject::IsKinematic, this), std::bind(&CPhysicalObject::SetKinematic, this, std::placeholders::_1));
	Helpers::GUIDrawer::DrawScalar<float>(p_root, "Mass", std::bind(&CPhysicalObject::GetMass, this), std::bind(&CPhysicalObject::SetMass, this, std::placeholders::_1), 0.1f, 0.f, 10000.f);
	Helpers::GUIDrawer::DrawScalar<float>(p_root, "Bounciness", std::bind(&CPhysicalObject::GetBounciness, this), std::bind(&CPhysicalObject::SetBounciness, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	Helpers::GUIDrawer::DrawScalar<float>(p_root, "Friction", std::bind(&CPhysicalObject::GetFriction, this), std::bind(&CPhysicalObject::SetFriction, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	Helpers::GUIDrawer::DrawVec3(p_root, "Linear Factor", std::bind(&CPhysicalObject::GetLinearFactor, this), std::bind(&CPhysicalObject::SetLinearFactor, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	Helpers::GUIDrawer::DrawVec3(p_root, "Angular Factor", std::bind(&CPhysicalObject::GetAngularFactor, this), std::bind(&CPhysicalObject::SetAngularFactor, this, std::placeholders::_1), 0.1f, 0.f, 1.f);
	
	Helpers::GUIDrawer::CreateTitle(p_root, "Collision Mode");
	auto& collisionMode = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(static_cast<int>(GetCollisionDetectionMode()));
	collisionMode.choices.emplace(0, "Discrete");
	collisionMode.choices.emplace(1, "Continuous");
	collisionMode.ValueChangedEvent += [this](int p_choice)
	{
		SetCollisionDetectionMode(static_cast<LittleEngine::Physics::Entities::PhysicalObject::ECollisionDetectionMode>(p_choice));
	};
}

void LittleEngine::CPhysicalObject::Init()
{
	m_physicalObject->SetEnabled(false);
}

void LittleEngine::CPhysicalObject::BindListener()
{
	/* Collision Events */
	m_physicalObject->CollisionStartEvent += [this](LittleEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		CollisionEnterEvent.Invoke(otherObject);
		owner->OnCollisionEnter(otherObject);
	};
	m_physicalObject->CollisionStayEvent += [this](LittleEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		CollisionStayEvent.Invoke(otherObject);
		owner->OnCollisionStay(otherObject);
	};
	m_physicalObject->CollisionStopEvent += [this](LittleEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		CollisionExitEvent.Invoke(otherObject);
		owner->OnCollisionExit(otherObject);
	};

	/* Trigger Events */
	m_physicalObject->TriggerStartEvent += [this](LittleEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		TriggerEnterEvent.Invoke(otherObject);
		owner->OnTriggerEnter(otherObject);
	};
	m_physicalObject->TriggerStayEvent += [this](LittleEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		TriggerStayEvent.Invoke(otherObject);
		owner->OnTriggerStay(otherObject);
	};
	m_physicalObject->TriggerStopEvent += [this](LittleEngine::Physics::Entities::PhysicalObject& otherPhysicalObject)
	{
		auto& otherObject = otherPhysicalObject.GetUserData<std::reference_wrapper<CPhysicalObject>>().get();

		TriggerExitEvent.Invoke(otherObject);
		owner->OnTriggerExit(otherObject);
	};
}

void LittleEngine::CPhysicalObject::OnEnable()
{
	m_physicalObject->SetEnabled(true);
}

void LittleEngine::CPhysicalObject::OnDisable()
{
	m_physicalObject->SetEnabled(false);
}
