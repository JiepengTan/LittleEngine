/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Selection/ComboBox.h"
#include "Modules/UI/Plugins/DataDispatcher.h"

#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Actor.h"


void LittleEngine::CCamera::DoInit(ActorPtr p_owner)
{
	Component::DoInit(p_owner);
	SetClearColor({ 0.1921569f, 0.3019608f, 0.4745098f });
}
std::string LittleEngine::CCamera::GetName()
{
	return "Camera";
}

void LittleEngine::CCamera::SetFov(float p_value)
{
	m_camera.SetFov(p_value);
}

void LittleEngine::CCamera::SetSize(float p_value)
{
    m_camera.SetSize(p_value);
}

void LittleEngine::CCamera::SetNear(float p_value)
{
	m_camera.SetNear(p_value);
}

void LittleEngine::CCamera::SetFar(float p_value)
{
	m_camera.SetFar(p_value);
}

void LittleEngine::CCamera::SetFrustumGeometryCulling(bool p_enable)
{
	m_camera.SetFrustumGeometryCulling(p_enable);
}

void LittleEngine::CCamera::SetFrustumLightCulling(bool p_enable)
{
	m_camera.SetFrustumLightCulling(p_enable);
}

void LittleEngine::CCamera::SetProjectionMode(LittleEngine::Rendering::Settings::EProjectionMode p_projectionMode)
{
    m_camera.SetProjectionMode(p_projectionMode);
}

float LittleEngine::CCamera::GetFov() const
{
	return m_camera.GetFov();
}

float LittleEngine::CCamera::GetSize() const
{
    return m_camera.GetSize();
}

float LittleEngine::CCamera::GetNear() const
{
	return m_camera.GetNear();
}

float LittleEngine::CCamera::GetFar() const
{
	return m_camera.GetFar();
}

const LittleEngine::FVector3 & LittleEngine::CCamera::GetClearColor() const
{
	return m_camera.GetClearColor();
}

bool LittleEngine::CCamera::HasFrustumGeometryCulling() const
{
	return m_camera.HasFrustumGeometryCulling();
}

void LittleEngine::CCamera::SetClearColor(const LittleEngine::FVector3 & p_clearColor)
{
	m_camera.SetClearColor(p_clearColor);
}

bool LittleEngine::CCamera::HasFrustumLightCulling() const
{
	return m_camera.HasFrustumLightCulling();
}

LittleEngine::Rendering::Settings::EProjectionMode LittleEngine::CCamera::GetProjectionMode() const
{
    return m_camera.GetProjectionMode();
}

LittleEngine::Rendering::LowRenderer::Camera & LittleEngine::CCamera::GetCamera()
{
	return m_camera;
}

void LittleEngine::CCamera::OnSerialize(ISerializer p_serializer)
{
	SerializeUtil::SerializeFloat("fov", m_camera.GetFov());
	SerializeUtil::SerializeFloat("size", m_camera.GetSize());
	SerializeUtil::SerializeFloat("near", m_camera.GetNear());
	SerializeUtil::SerializeFloat("far", m_camera.GetFar());
	SerializeUtil::SerializeVec3("clear_color", m_camera.GetClearColor());
	SerializeUtil::SerializeBoolean("frustum_geometry_culling", m_camera.HasFrustumGeometryCulling());
	SerializeUtil::SerializeBoolean("frustum_light_culling", m_camera.HasFrustumLightCulling());
	SerializeUtil::SerializeInt("projection_mode", static_cast<int>(m_camera.GetProjectionMode()));
}

void LittleEngine::CCamera::OnDeserialize(ISerializer p_serializer)
{
	m_camera.SetFov(SerializeUtil::DeserializeFloat("fov"));
	m_camera.SetSize(SerializeUtil::DeserializeFloat("size"));
	m_camera.SetNear(SerializeUtil::DeserializeFloat("near"));
	m_camera.SetFar(SerializeUtil::DeserializeFloat("far"));
	m_camera.SetClearColor(SerializeUtil::DeserializeVec3("clear_color"));
	m_camera.SetFrustumGeometryCulling(SerializeUtil::DeserializeBoolean("frustum_geometry_culling"));
	m_camera.SetFrustumLightCulling(SerializeUtil::DeserializeBoolean("frustum_light_culling"));
    m_camera.SetProjectionMode(static_cast<LittleEngine::Rendering::Settings::EProjectionMode>(SerializeUtil::DeserializeInt("projection_mode")));
}

void LittleEngine::CCamera::OnInspector()
{
    auto currentProjectionMode = GetProjectionMode();

	GUIUtil::DrawScalar<float>( "Field of view", std::bind(&CCamera::GetFov, this), std::bind(&CCamera::SetFov, this, std::placeholders::_1));
    auto& fovWidget = *GUIUtil::GetWidgets()[GUIUtil::GetWidgets().size() - 1].first;
    auto& fovWidgetLabel = *GUIUtil::GetWidgets()[GUIUtil::GetWidgets().size() - 2].first;
    fovWidget.enabled = fovWidgetLabel.enabled = currentProjectionMode == LittleEngine::Rendering::Settings::EProjectionMode::PERSPECTIVE;

	GUIUtil::DrawScalar<float>( "Size", std::bind(&CCamera::GetSize, this), std::bind(&CCamera::SetSize, this, std::placeholders::_1));
    auto& sizeWidget = *GUIUtil::GetWidgets()[GUIUtil::GetWidgets().size() - 1].first;
    auto& sizeWidgetLabel = *GUIUtil::GetWidgets()[GUIUtil::GetWidgets().size() - 2].first;
    sizeWidget.enabled = sizeWidgetLabel.enabled = currentProjectionMode == LittleEngine::Rendering::Settings::EProjectionMode::ORTHOGRAPHIC;

	GUIUtil::DrawScalar<float>( "Near", std::bind(&CCamera::GetNear, this), std::bind(&CCamera::SetNear, this, std::placeholders::_1));
	GUIUtil::DrawScalar<float>( "Far", std::bind(&CCamera::GetFar, this), std::bind(&CCamera::SetFar, this, std::placeholders::_1));
	GUIUtil::DrawColor( "Clear color", [this]() {return reinterpret_cast<const LittleEngine::Color&>(GetClearColor()); }, [this](LittleEngine::Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);
	GUIUtil::DrawBoolean( "Frustum Geometry Culling", std::bind(&CCamera::HasFrustumGeometryCulling, this), std::bind(&CCamera::SetFrustumGeometryCulling, this, std::placeholders::_1));
	GUIUtil::DrawBoolean( "Frustum Light Culling", std::bind(&CCamera::HasFrustumLightCulling, this), std::bind(&CCamera::SetFrustumLightCulling, this, std::placeholders::_1));

    GUIUtil::CreateTitle( "Projection Mode");
    auto& projectionMode = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(static_cast<int>(GetProjectionMode()));
    projectionMode.choices.emplace(0, "Orthographic");
    projectionMode.choices.emplace(1, "Perspective");
    projectionMode.ValueChangedEvent += [this, &fovWidget, &fovWidgetLabel, &sizeWidget, &sizeWidgetLabel](int p_choice)
    {
        const auto newProjectionMode = static_cast<LittleEngine::Rendering::Settings::EProjectionMode>(p_choice);
        SetProjectionMode(newProjectionMode);
        fovWidget.enabled = fovWidgetLabel.enabled = newProjectionMode == LittleEngine::Rendering::Settings::EProjectionMode::PERSPECTIVE;
        sizeWidget.enabled = sizeWidgetLabel.enabled = newProjectionMode == LittleEngine::Rendering::Settings::EProjectionMode::ORTHOGRAPHIC;
    };
}
