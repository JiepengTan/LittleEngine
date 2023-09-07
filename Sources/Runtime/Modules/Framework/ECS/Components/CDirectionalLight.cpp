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


void LittleEngine::CDirectionalLight::DoInit(ActorPtr p_owner)
{
	CLight::DoInit(p_owner);
	m_data.type = static_cast<float>(LittleEngine::Rendering::Entities::Light::Type::DIRECTIONAL);
}

std::string LittleEngine::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void LittleEngine::CDirectionalLight::OnSerialize(ISerializer p_serializer)
{
	CLight::OnSerialize(p_serializer);
}

void LittleEngine::CDirectionalLight::OnDeserialize(ISerializer p_serializer)
{
	CLight::OnDeserialize(p_serializer);
}

void LittleEngine::CDirectionalLight::OnInspector()
{
	CLight::OnInspector();
}
