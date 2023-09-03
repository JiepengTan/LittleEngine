/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Layout/Group.h"

#include "GamePlay/ECS/Actor.h"

#include "GamePlay/ECS/Components/CLight.h"

OvCore::ECS::Components::CLight::CLight(ECS::Actor & p_owner) :
	AComponent(p_owner),
	m_data(p_owner.transform.GetFTransform(), {})
{
}

const OvRendering::Entities::Light& OvCore::ECS::Components::CLight::GetData() const
{
	return m_data;
}

const OvMaths::FVector3& OvCore::ECS::Components::CLight::GetColor() const
{
	return m_data.color;
}

float OvCore::ECS::Components::CLight::GetIntensity() const
{
	return m_data.intensity;
}

void OvCore::ECS::Components::CLight::SetColor(const OvMaths::FVector3& p_color)
{
	m_data.color = p_color;
}

void OvCore::ECS::Components::CLight::SetIntensity(float p_intensity)
{
	m_data.intensity = p_intensity;
}
OvRendering::Entities::Light::Type OvCore::ECS::Components::CLight::GetLightType()
{
	return static_cast<OvRendering::Entities::Light::Type>(m_data.type);
}
bool OvCore::ECS::Components::CLight::IsDirectional()
{
	return GetLightType() == OvRendering::Entities::Light::Type::DIRECTIONAL;
}
void OvCore::ECS::Components::CLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace OvCore::Helpers;

	Serializer::SerializeVec3(p_doc, p_node, "color", m_data.color);
	Serializer::SerializeFloat(p_doc, p_node, "intensity", m_data.intensity);
}

void OvCore::ECS::Components::CLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace OvCore::Helpers;

	Serializer::DeserializeVec3(p_doc, p_node, "color", m_data.color);
	Serializer::DeserializeFloat(p_doc, p_node, "intensity", m_data.intensity);
}

void OvCore::ECS::Components::CLight::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	using namespace OvCore::Helpers;

	GUIDrawer::DrawColor(p_root, "Color", reinterpret_cast<OvUI::Types::Color&>(m_data.color));
	GUIDrawer::DrawScalar<float>(p_root, "Intensity", m_data.intensity, 0.005f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);
}
