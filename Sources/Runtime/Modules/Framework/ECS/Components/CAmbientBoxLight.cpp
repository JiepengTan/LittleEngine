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

#include "Modules/Framework/ECS/Components/CAmbientBoxLight.h"

void LittleEngine::CAmbientBoxLight::OnAwake()
{
	CLight::OnAwake();
	m_data.type = static_cast<float>(LittleEngine::Rendering::Entities::Light::Type::AMBIENT_BOX);
	m_data.intensity = 0.1f;
	m_data.constant = 1.0f;
	m_data.linear = 1.0f;
	m_data.quadratic = 1.0f;
}

std::string LittleEngine::CAmbientBoxLight::GetName()
{
	return "Ambient Box Light";
}

LittleEngine::FVector3 LittleEngine::CAmbientBoxLight::GetSize() const
{
	return { m_data.constant, m_data.linear, m_data.quadratic };
}

void LittleEngine::CAmbientBoxLight::SetSize(const LittleEngine::FVector3& p_size)
{
	m_data.constant = p_size.x;
	m_data.linear = p_size.y;
	m_data.quadratic = p_size.z;
}

void LittleEngine::CAmbientBoxLight::OnSerialize(ISerializer p_serializer)
{
	

	CLight::OnSerialize(p_serializer);

	SerializeUtil::SerializeVec3("size", { m_data.constant, m_data.linear, m_data.quadratic });
}

void LittleEngine::CAmbientBoxLight::OnDeserialize(ISerializer p_serializer)
{
	

	CLight::OnDeserialize(p_serializer);

	LittleEngine::FVector3 size = SerializeUtil::DeserializeVec3("size");
	m_data.constant = size.x;
	m_data.linear = size.y;
	m_data.quadratic = size.z;
}

void LittleEngine::CAmbientBoxLight::OnInspector()
{
	CLight::OnInspector();

	auto sizeGatherer = [this]() -> LittleEngine::FVector3 { return { m_data.constant, m_data.linear, m_data.quadratic }; };
	auto sizeProvider = [this](const LittleEngine::FVector3& p_data) { m_data.constant = p_data.x; m_data.linear = p_data.y, m_data.quadratic = p_data.z; };

	GUIUtil::DrawVec3( "Size", sizeGatherer, sizeProvider, 0.1f, 0.f);
}
