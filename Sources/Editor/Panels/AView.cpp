/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>

#include "../Editor/Panels/AView.h"
#include "../Editor/Core/EditorActions.h"

LittleEditor::Panels::AView::AView
(
	const std::string& p_title,
	bool p_opened,
	const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings), m_editorRenderer(EDITOR_RENDERER())
{
	m_cameraPosition = { -10.0f, 3.0f, 10.0f };
	m_cameraRotation = LittleEngine::FQuaternion({0.0f, 135.0f, 0.0f});

	m_image = &CreateWidget<LittleEngine::UI::Widgets::Visual::Image>(m_fbo.GetTextureID(), LittleEngine::FVector2{ 0.f, 0.f });

    scrollable = false;
}

void LittleEditor::Panels::AView::Update(float p_deltaTime)
{
	auto[winWidth, winHeight] = GetSafeSize();

	m_image->size = LittleEngine::FVector2(static_cast<float>(winWidth), static_cast<float>(winHeight));

	m_fbo.Resize(winWidth, winHeight);
}

void LittleEditor::Panels::AView::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	LittleEngine::UI::Panels::PanelWindow::_Draw_Impl();

	ImGui::PopStyleVar();
}

void LittleEditor::Panels::AView::Render()
{
	m_camera.m_CameraType = LittleEngine::Rendering::Settings::ECameraType::Game;
	FillEngineUBO();

	auto [winWidth, winHeight] = GetSafeSize();

	EDITOR_CONTEXT(shapeDrawer)->SetViewProjection(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());

	EDITOR_CONTEXT(renderer)->SetViewPort(0, 0, winWidth, winHeight);

	_Render_Impl();
}

void LittleEditor::Panels::AView::SetCameraPosition(const LittleEngine::FVector3 & p_position)
{
	m_cameraPosition = p_position;
}

void LittleEditor::Panels::AView::SetCameraRotation(const LittleEngine::FQuaternion& p_rotation)
{
	m_cameraRotation = p_rotation;
}

const LittleEngine::FVector3 & LittleEditor::Panels::AView::GetCameraPosition() const
{
	return m_cameraPosition;
}

const LittleEngine::FQuaternion& LittleEditor::Panels::AView::GetCameraRotation() const
{
	return m_cameraRotation;
}

LittleEngine::Rendering::LowRenderer::Camera & LittleEditor::Panels::AView::GetCamera()
{
	return m_camera;
}

std::pair<uint16_t, uint16_t> LittleEditor::Panels::AView::GetSafeSize() const
{
	auto result = GetSize() - LittleEngine::FVector2{ 0.f, 25.f }; // 25 == title bar height
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

const LittleEngine::FVector3& LittleEditor::Panels::AView::GetGridColor() const
{
	return m_gridColor;
}

void LittleEditor::Panels::AView::SetGridColor(const LittleEngine::FVector3& p_color)
{
	m_gridColor = p_color;
}

void LittleEditor::Panels::AView::FillEngineUBO()
{
	auto& engineUBO = *EDITOR_CONTEXT(engineUBO);

	auto[winWidth, winHeight] = GetSafeSize();

	size_t offset = sizeof(LittleEngine::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)
	engineUBO.SetSubData(LittleEngine::FMatrix4::Transpose(m_camera.GetViewMatrix()), std::ref(offset));
	engineUBO.SetSubData(LittleEngine::FMatrix4::Transpose(m_camera.GetProjectionMatrix()), std::ref(offset));
	engineUBO.SetSubData(m_cameraPosition, std::ref(offset));
}

void LittleEditor::Panels::AView::PrepareCamera()
{
	auto [winWidth, winHeight] = GetSafeSize();
	m_camera.CacheMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
	m_camera.m_CameraType = LittleEngine::Rendering::Settings::ECameraType::Asset;
}
