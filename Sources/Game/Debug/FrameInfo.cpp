/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#ifdef _DEBUG

#include "../Game/Debug/FrameInfo.h"

LittleGame::Debug::FrameInfo::FrameInfo(LittleEngine::Rendering::Core::Renderer& p_renderer, LittleEngine::Windowing::Window& p_window) :
	m_renderer(p_renderer),
	m_window(p_window)
{
	m_defaultHorizontalAlignment = LittleEngine::UI::Settings::EHorizontalAlignment::LEFT;
	m_defaultVerticalAlignment = LittleEngine::UI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	m_frameInfo[0] = &CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("", LittleEngine::UI::Types::Color::Yellow);
	m_frameInfo[1] = &CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("", LittleEngine::UI::Types::Color::Yellow);
	m_frameInfo[2] = &CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("", LittleEngine::UI::Types::Color::Yellow);
}

void LittleGame::Debug::FrameInfo::Update(float p_deltaTime)
{
	auto& frameInfo = m_renderer.GetFrameInfo();

	m_frameInfo[0]->content = "Triangles: " + std::to_string(frameInfo.polyCount);
	m_frameInfo[1]->content = "Batches: " + std::to_string(frameInfo.batchCount);
	m_frameInfo[2]->content = "Instances: " + std::to_string(frameInfo.instanceCount);

	SetPosition({ 10.0f , static_cast<float>(m_window.GetSize().second) - 10.f });
	SetAlignment(LittleEngine::UI::Settings::EHorizontalAlignment::LEFT, LittleEngine::UI::Settings::EVerticalAlignment::BOTTOM);
}

#endif
