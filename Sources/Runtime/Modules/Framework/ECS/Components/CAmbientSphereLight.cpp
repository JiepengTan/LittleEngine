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

void LittleEngine::CAmbientSphereLight::OnAwake()
{
	CLight::OnAwake();
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

void LittleEngine::CAmbientSphereLight::OnSerialize(ISerializer p_serializer)
{
	

	CLight::OnSerialize(p_serializer);

	SerializeUtil::SerializeFloat("radius", m_data.constant);
}

void LittleEngine::CAmbientSphereLight::OnDeserialize(ISerializer p_serializer)
{
	

	CLight::OnDeserialize(p_serializer);

	SerializeUtil::DeserializeFloat("radius", m_data.constant);
}

void LittleEngine::CAmbientSphereLight::OnInspector()
{
	

	CLight::OnInspector();

	GUIUtil::DrawScalar<float>( "Radius", m_data.constant, 0.1f, 0.f);
}
