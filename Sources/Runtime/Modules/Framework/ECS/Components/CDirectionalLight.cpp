/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"

#include "Modules/Framework/ECS/Actor.h"

#include "Modules/Framework/ECS/Components/CDirectionalLight.h"

LittleEngine::CDirectionalLight::CDirectionalLight(Actor & p_owner) :
	CLight(p_owner)
{
	m_data.type = static_cast<float>(LittleEngine::Rendering::Entities::Light::Type::DIRECTIONAL);
}

std::string LittleEngine::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void LittleEngine::CDirectionalLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnSerialize(p_doc, p_node);
}

void LittleEngine::CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnDeserialize(p_doc, p_node);
}

void LittleEngine::CDirectionalLight::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
}
