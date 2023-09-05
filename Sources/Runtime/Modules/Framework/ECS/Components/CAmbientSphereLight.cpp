/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"
#include "Modules/UI/Widgets/Buttons/Button.h"

#include "Modules/Framework/ECS/Actor.h"

#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"

LittleEngine::CAmbientSphereLight::CAmbientSphereLight(Actor & p_owner) :
	CLight(p_owner)
{
    m_data.intensity = 0.1f;
	m_data.constant = 1.0f;

	m_data.type = static_cast<float>(LittleEngine::Rendering::Entities::Light::Type::AMBIENT_SPHERE);
}

std::string LittleEngine::CAmbientSphereLight::GetName()
{
	return "Ambient Sphere Light";
}

float LittleEngine::CAmbientSphereLight::GetRadius() const
{
	return m_data.quadratic;
}

void LittleEngine::CAmbientSphereLight::SetRadius(float p_radius)
{
	m_data.constant = p_radius;
}

void LittleEngine::CAmbientSphereLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace LittleEngine::Helpers;

	CLight::OnSerialize(p_doc, p_node);

	Serializer::SerializeFloat(p_doc, p_node, "radius", m_data.constant);
}

void LittleEngine::CAmbientSphereLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace LittleEngine::Helpers;

	CLight::OnDeserialize(p_doc, p_node);

	Serializer::DeserializeFloat(p_doc, p_node, "radius", m_data.constant);
}

void LittleEngine::CAmbientSphereLight::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
{
	using namespace LittleEngine::Helpers;

	CLight::OnInspector(p_root);

	GUIDrawer::DrawScalar<float>(p_root, "Radius", m_data.constant, 0.1f, 0.f);
}
