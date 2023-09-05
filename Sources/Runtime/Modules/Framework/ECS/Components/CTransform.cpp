/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Components/CTransform.h"

LittleEngine::CTransform::CTransform(Actor& p_owner, LittleEngine::FVector3 p_localPosition, LittleEngine::FQuaternion p_localRotation, LittleEngine::FVector3 p_localScale) :
AComponent(p_owner)
{
	m_transform.GenerateMatricesLocal(p_localPosition, p_localRotation, p_localScale);
}

std::string LittleEngine::CTransform::GetName()
{
	return "Transform";
}

void LittleEngine::CTransform::SetParent(CTransform& p_parent)
{
	m_transform.SetParent(p_parent.GetFTransform());
}

bool LittleEngine::CTransform::RemoveParent()
{
	return m_transform.RemoveParent();
}

bool LittleEngine::CTransform::HasParent() const
{
	return m_transform.HasParent();
}

void LittleEngine::CTransform::SetWorldMatrix(LittleEngine::FMatrix4 p_worldMatrix)
{
	m_transform.SetWorldMatrix(p_worldMatrix);
}
void LittleEngine::CTransform::SetLocalMatrix(LittleEngine::FMatrix4 p_localMatrix)
{
	m_transform.SetLocalMatrix(p_localMatrix);
}
void LittleEngine::CTransform::SetLocalPosition(LittleEngine::FVector3 p_newPosition)
{
	m_transform.SetLocalPosition(p_newPosition);
}

void LittleEngine::CTransform::SetLocalRotation(LittleEngine::FQuaternion p_newRotation)
{
	m_transform.SetLocalRotation(p_newRotation);
}

void LittleEngine::CTransform::SetLocalScale(LittleEngine::FVector3 p_newScale)
{
	m_transform.SetLocalScale(p_newScale);
}
void LittleEngine::CTransform::SetWorldPosition(LittleEngine::FVector3 p_newPosition)
{
	m_transform.SetWorldPosition(p_newPosition);
}

void LittleEngine::CTransform::SetWorldRotation(LittleEngine::FQuaternion p_newRotation)
{
	m_transform.SetWorldRotation(p_newRotation);
}

void LittleEngine::CTransform::SetWorldScale(LittleEngine::FVector3 p_newScale)
{
	m_transform.SetWorldScale(p_newScale);
}

void LittleEngine::CTransform::TranslateLocal(const LittleEngine::FVector3 & p_translation)
{
	m_transform.TranslateLocal(p_translation);
}

void LittleEngine::CTransform::RotateLocal(const LittleEngine::FQuaternion & p_rotation)
{
	m_transform.RotateLocal(p_rotation);
}

void LittleEngine::CTransform::ScaleLocal(const LittleEngine::FVector3 & p_scale)
{
	m_transform.ScaleLocal(p_scale);
}

const LittleEngine::FVector3 & LittleEngine::CTransform::GetLocalPosition() const
{
	return m_transform.GetLocalPosition();
}

const LittleEngine::FQuaternion & LittleEngine::CTransform::GetLocalRotation() const
{
	return m_transform.GetLocalRotation();
}

const LittleEngine::FVector3 & LittleEngine::CTransform::GetLocalScale() const
{
	return m_transform.GetLocalScale();
}

const LittleEngine::FVector3 & LittleEngine::CTransform::GetWorldPosition() const
{
	return m_transform.GetWorldPosition();
}

const LittleEngine::FQuaternion & LittleEngine::CTransform::GetWorldRotation() const
{
	return m_transform.GetWorldRotation();
}

const LittleEngine::FVector3 & LittleEngine::CTransform::GetWorldScale() const
{
	return m_transform.GetWorldScale();
}

const LittleEngine::FMatrix4 & LittleEngine::CTransform::GetLocalMatrix() const
{
	return m_transform.GetLocalMatrix();
}

const LittleEngine::FMatrix4 & LittleEngine::CTransform::GetWorldMatrix() const
{
	return m_transform.GetWorldMatrix();
}

LittleEngine::FTransform & LittleEngine::CTransform::GetFTransform()
{
	return m_transform;
}

LittleEngine::FVector3 LittleEngine::CTransform::GetWorldForward() const
{
	return m_transform.GetWorldForward();
}

LittleEngine::FVector3 LittleEngine::CTransform::GetWorldUp() const
{
	return m_transform.GetWorldUp();
}

LittleEngine::FVector3 LittleEngine::CTransform::GetWorldRight() const
{
	return m_transform.GetWorldRight();
}

LittleEngine::FVector3 LittleEngine::CTransform::GetLocalForward() const
{
	return m_transform.GetLocalForward();
}

LittleEngine::FVector3 LittleEngine::CTransform::GetLocalUp() const
{
	return m_transform.GetLocalUp();
}

LittleEngine::FVector3 LittleEngine::CTransform::GetLocalRight() const
{
	return m_transform.GetLocalRight();
}

void LittleEngine::CTransform::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	LittleEngine::Serializer::SerializeVec3(p_doc, p_node, "position", GetLocalPosition());
	LittleEngine::Serializer::SerializeQuat(p_doc, p_node, "rotation", GetLocalRotation());
	LittleEngine::Serializer::SerializeVec3(p_doc, p_node, "scale", GetLocalScale());
}

void LittleEngine::CTransform::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	m_transform.GenerateMatricesLocal
	(
		LittleEngine::Serializer::DeserializeVec3(p_doc, p_node, "position"),
		LittleEngine::Serializer::DeserializeQuat(p_doc, p_node, "rotation"),
		LittleEngine::Serializer::DeserializeVec3(p_doc, p_node, "scale")
	);
}

void LittleEngine::CTransform::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
{
	auto getRotation = [this]
	{ 
		return LittleEngine::FQuaternion::EulerAngles(GetLocalRotation());
	};

	auto setRotation = [this](LittleEngine::FVector3 result)
	{
		SetLocalRotation(LittleEngine::FQuaternion(result));
	};

	LittleEngine::Helpers::GUIDrawer::DrawVec3(p_root, "Position", std::bind(&CTransform::GetLocalPosition, this), std::bind(&CTransform::SetLocalPosition, this, std::placeholders::_1), 0.05f);
	LittleEngine::Helpers::GUIDrawer::DrawVec3(p_root, "Rotation", getRotation, setRotation, 0.05f);
	LittleEngine::Helpers::GUIDrawer::DrawVec3(p_root, "Scale", std::bind(&CTransform::GetLocalScale, this), std::bind(&CTransform::SetLocalScale, this, std::placeholders::_1), 0.05f, 0.0001f);
}
