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

#include "Modules/Framework/ECS/Actor.h"

#include "Modules/Framework/ECS/Components/CLight.h"



void LittleEngine::CLight::OnAwake()
{
	m_data.DoInit (owner->transform->GetFTransform());
}


const LittleEngine::Rendering::Entities::Light& LittleEngine::CLight::GetData() const
{
	return m_data;
}

const LittleEngine::FVector3& LittleEngine::CLight::GetColor() const
{
	return m_data.color;
}

float LittleEngine::CLight::GetIntensity() const
{
	return m_data.intensity;
}

void LittleEngine::CLight::SetColor(const LittleEngine::FVector3& p_color)
{
	m_data.color = p_color;
}

void LittleEngine::CLight::SetIntensity(float p_intensity)
{
	m_data.intensity = p_intensity;
}

void LittleEngine::CLight::OnAfterSceneLoaded(ActorPtr p_actor)
{
	Component::OnAfterSceneLoaded(p_actor);
	m_data.DoInit(p_actor->transform->GetFTransform());
}

LittleEngine::Rendering::Entities::Light::Type LittleEngine::CLight::GetLightType()
{
	return static_cast<LittleEngine::Rendering::Entities::Light::Type>(m_data.type);
}
bool LittleEngine::CLight::IsDirectional()
{
	return GetLightType() == LittleEngine::Rendering::Entities::Light::Type::DIRECTIONAL;
}

void LittleEngine::CLight::OnInspector()
{
	

	GUIUtil::DrawColor( "Color", reinterpret_cast<LittleEngine::Color&>(m_data.color));
	GUIUtil::DrawScalar<float>( "Intensity", m_data.intensity, 0.005f, GUIUtil::_MIN_FLOAT, GUIUtil::_MAX_FLOAT);
}
