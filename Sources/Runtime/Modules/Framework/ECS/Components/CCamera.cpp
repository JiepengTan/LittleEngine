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

LittleEngine::CCamera::CCamera(Actor& p_owner) : AComponent(p_owner)
{
	/* Default clear color for the CCamera (Different from Camera default clear color) */
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

void LittleEngine::CCamera::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	LittleEngine::Serializer::SerializeFloat(p_doc, p_node, "fov", m_camera.GetFov());
	LittleEngine::Serializer::SerializeFloat(p_doc, p_node, "size", m_camera.GetSize());
	LittleEngine::Serializer::SerializeFloat(p_doc, p_node, "near", m_camera.GetNear());
	LittleEngine::Serializer::SerializeFloat(p_doc, p_node, "far", m_camera.GetFar());
	LittleEngine::Serializer::SerializeVec3(p_doc, p_node, "clear_color", m_camera.GetClearColor());
	LittleEngine::Serializer::SerializeBoolean(p_doc, p_node, "frustum_geometry_culling", m_camera.HasFrustumGeometryCulling());
	LittleEngine::Serializer::SerializeBoolean(p_doc, p_node, "frustum_light_culling", m_camera.HasFrustumLightCulling());
	LittleEngine::Serializer::SerializeInt(p_doc, p_node, "projection_mode", static_cast<int>(m_camera.GetProjectionMode()));
}

void LittleEngine::CCamera::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	m_camera.SetFov(LittleEngine::Serializer::DeserializeFloat(p_doc, p_node, "fov"));
	m_camera.SetSize(LittleEngine::Serializer::DeserializeFloat(p_doc, p_node, "size"));
	m_camera.SetNear(LittleEngine::Serializer::DeserializeFloat(p_doc, p_node, "near"));
	m_camera.SetFar(LittleEngine::Serializer::DeserializeFloat(p_doc, p_node, "far"));
	m_camera.SetClearColor(LittleEngine::Serializer::DeserializeVec3(p_doc, p_node, "clear_color"));
	m_camera.SetFrustumGeometryCulling(LittleEngine::Serializer::DeserializeBoolean(p_doc, p_node, "frustum_geometry_culling"));
	m_camera.SetFrustumLightCulling(LittleEngine::Serializer::DeserializeBoolean(p_doc, p_node, "frustum_light_culling"));

    // We have to make sure the "projection_mode" exists in the serialized component, otherwise we do not want to modify the default setting (Perspective).
    // This is a bad practice to have each components calling setters in `OnDeserialize` even if no XML node hasn't been found for a given property.
    // We should rework this system later. As it is out of the scope of the orthographic projection scope, this will be left as is for now.
    if (p_node->FirstChildElement("projection_mode"))
    {
        m_camera.SetProjectionMode(static_cast<LittleEngine::Rendering::Settings::EProjectionMode>(LittleEngine::Serializer::DeserializeInt(p_doc, p_node, "projection_mode")));
    }
}

void LittleEngine::CCamera::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
{
    auto currentProjectionMode = GetProjectionMode();

	LittleEngine::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Field of view", std::bind(&CCamera::GetFov, this), std::bind(&CCamera::SetFov, this, std::placeholders::_1));
    auto& fovWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1].first;
    auto& fovWidgetLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2].first;
    fovWidget.enabled = fovWidgetLabel.enabled = currentProjectionMode == LittleEngine::Rendering::Settings::EProjectionMode::PERSPECTIVE;

	LittleEngine::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Size", std::bind(&CCamera::GetSize, this), std::bind(&CCamera::SetSize, this, std::placeholders::_1));
    auto& sizeWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1].first;
    auto& sizeWidgetLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2].first;
    sizeWidget.enabled = sizeWidgetLabel.enabled = currentProjectionMode == LittleEngine::Rendering::Settings::EProjectionMode::ORTHOGRAPHIC;

	LittleEngine::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Near", std::bind(&CCamera::GetNear, this), std::bind(&CCamera::SetNear, this, std::placeholders::_1));
	LittleEngine::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Far", std::bind(&CCamera::GetFar, this), std::bind(&CCamera::SetFar, this, std::placeholders::_1));
	LittleEngine::Helpers::GUIDrawer::DrawColor(p_root, "Clear color", [this]() {return reinterpret_cast<const LittleEngine::UI::Types::Color&>(GetClearColor()); }, [this](LittleEngine::UI::Types::Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);
	LittleEngine::Helpers::GUIDrawer::DrawBoolean(p_root, "Frustum Geometry Culling", std::bind(&CCamera::HasFrustumGeometryCulling, this), std::bind(&CCamera::SetFrustumGeometryCulling, this, std::placeholders::_1));
	LittleEngine::Helpers::GUIDrawer::DrawBoolean(p_root, "Frustum Light Culling", std::bind(&CCamera::HasFrustumLightCulling, this), std::bind(&CCamera::SetFrustumLightCulling, this, std::placeholders::_1));

    Helpers::GUIDrawer::CreateTitle(p_root, "Projection Mode");
    auto& projectionMode = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(static_cast<int>(GetProjectionMode()));
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
