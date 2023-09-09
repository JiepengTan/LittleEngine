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

#include "Modules/Framework/ECS/Components/CPointLight.h"


void LittleEngine::CPointLight::OnAwake()
{
	CLight::OnAwake();
	m_data.type = static_cast<float>(LittleEngine::Rendering::Entities::Light::Type::POINT);
}



std::string LittleEngine::CPointLight::GetName()
{
	return "Point Light";
}

float LittleEngine::CPointLight::GetConstant() const
{
	return m_data.constant;
}

float LittleEngine::CPointLight::GetLinear() const
{
	return m_data.linear;
}

float LittleEngine::CPointLight::GetQuadratic() const
{
	return m_data.quadratic;
}

void LittleEngine::CPointLight::SetConstant(float p_constant)
{
	m_data.constant = p_constant;
}

void LittleEngine::CPointLight::SetLinear(float p_linear)
{
	m_data.linear = p_linear;
}

void LittleEngine::CPointLight::SetQuadratic(float p_quadratic)
{
	m_data.quadratic = p_quadratic;
}

void LittleEngine::CPointLight::OnSerialize(ISerializer p_serializer)
{
	

	CLight::OnSerialize(p_serializer);

	SerializeUtil::SerializeFloat("constant", m_data.constant);
	SerializeUtil::SerializeFloat("linear", m_data.linear);
	SerializeUtil::SerializeFloat("quadratic", m_data.quadratic);
}

void LittleEngine::CPointLight::OnDeserialize(ISerializer p_serializer)
{
	

	CLight::OnDeserialize(p_serializer);

	SerializeUtil::DeserializeFloat("constant", m_data.constant);
	SerializeUtil::DeserializeFloat("linear", m_data.linear);
	SerializeUtil::DeserializeFloat("quadratic", m_data.quadratic);
}

void LittleEngine::CPointLight::OnInspector()
{
	

	CLight::OnInspector();

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
