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

#include "Modules/Framework/ECS/Components/CSpotLight.h"


void LittleEngine::CSpotLight::DoInit(ActorPtr p_owner)
{
	CLight::DoInit(p_owner);
	m_data.type = static_cast<float>(LittleEngine::Rendering::Entities::Light::Type::SPOT);
}

std::string LittleEngine::CSpotLight::GetName()
{
	return "Spot Light";
}

float LittleEngine::CSpotLight::GetConstant() const
{
	return m_data.constant;
}

float LittleEngine::CSpotLight::GetLinear() const
{
	return m_data.linear;
}

float LittleEngine::CSpotLight::GetQuadratic() const
{
	return m_data.quadratic;
}

float LittleEngine::CSpotLight::GetCutoff() const
{
	return m_data.cutoff;
}

float LittleEngine::CSpotLight::GetOuterCutoff() const
{
	return m_data.outerCutoff;
}

void LittleEngine::CSpotLight::SetConstant(float p_constant)
{
	m_data.constant = p_constant;
}

void LittleEngine::CSpotLight::SetLinear(float p_linear)
{
	m_data.linear = p_linear;
}

void LittleEngine::CSpotLight::SetQuadratic(float p_quadratic)
{
	m_data.quadratic = p_quadratic;
}

void LittleEngine::CSpotLight::SetCutoff(float p_cutoff)
{
	m_data.cutoff = p_cutoff;
}

void LittleEngine::CSpotLight::SetOuterCutoff(float p_outerCutoff)
{
	m_data.outerCutoff = p_outerCutoff;
}

void LittleEngine::CSpotLight::OnSerialize(ISerializer p_serializer)
{
	

	CLight::OnSerialize(p_serializer);

	SerializeUtil::SerializeFloat("constant", m_data.constant);
	SerializeUtil::SerializeFloat("linear", m_data.linear);
	SerializeUtil::SerializeFloat("quadratic", m_data.quadratic);
	SerializeUtil::SerializeFloat("cutoff", m_data.cutoff);
	SerializeUtil::SerializeFloat("outercutoff", m_data.outerCutoff);
}

void LittleEngine::CSpotLight::OnDeserialize(ISerializer p_serializer)
{
	

	CLight::OnDeserialize(p_serializer);

	SerializeUtil::DeserializeFloat("constant", m_data.constant);
	SerializeUtil::DeserializeFloat("linear", m_data.linear);
	SerializeUtil::DeserializeFloat("quadratic", m_data.quadratic);
	SerializeUtil::DeserializeFloat("cutoff", m_data.cutoff);
	SerializeUtil::DeserializeFloat("outercutoff", m_data.outerCutoff);
}

void LittleEngine::CSpotLight::OnInspector()
{
	

	CLight::OnInspector();

	GUIUtil::DrawScalar<float>( "Cut-Off", m_data.cutoff, 0.1f, 0.f, 180.f);
	GUIUtil::DrawScalar<float>( "Outer Cut-Off", m_data.outerCutoff, 0.1f, 0.f, 180.f);

	GUIUtil::CreateTitle( "Fall-off presets");
	auto& presetsRoot = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& constantPreset = presetsRoot.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Constant");
	constantPreset.ClickedEvent += [this] { m_data.constant = 1.f, m_data.linear = m_data.quadratic = 0.f; };
	constantPreset.lineBreak = false;
	constantPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& linearPreset = presetsRoot.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Linear");
	linearPreset.ClickedEvent += [this] { m_data.linear = 1.f, m_data.constant = m_data.quadratic = 0.f; };
	linearPreset.lineBreak = false;
	linearPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	auto& quadraticPreset = presetsRoot.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Quadratic");
	quadraticPreset.ClickedEvent += [this] { m_data.quadratic = 1.f, m_data.constant = m_data.linear = 0.f; };
	quadraticPreset.idleBackgroundColor = { 0.7f, 0.5f, 0.f };

	GUIUtil::DrawScalar<float>( "Constant", m_data.constant, 0.005f, 0.f);
	GUIUtil::DrawScalar<float>( "Linear", m_data.linear, 0.005f, 0.f);
	GUIUtil::DrawScalar<float>( "Quadratic", m_data.quadratic, 0.005f, 0.f);
}
