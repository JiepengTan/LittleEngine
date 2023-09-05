/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#ifdef _DEBUG

#include "../Game/Debug/DriverInfo.h"

LittleGame::Debug::DriverInfo::DriverInfo(LittleEngine::Rendering::Core::Renderer& p_renderer, LittleEngine::Windowing::Window& p_window)
{
	m_defaultHorizontalAlignment = LittleEngine::UI::Settings::EHorizontalAlignment::RIGHT;
	m_defaultVerticalAlignment = LittleEngine::UI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Vendor: "	+ p_renderer.GetString(GL_VENDOR), LittleEngine::UI::Types::Color::Yellow);
	CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Hardware: " + p_renderer.GetString(GL_RENDERER), LittleEngine::UI::Types::Color::Yellow);
	CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("OpenGL Version: " + p_renderer.GetString(GL_VERSION), LittleEngine::UI::Types::Color::Yellow);
	CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("GLSL Version: " + p_renderer.GetString(GL_SHADING_LANGUAGE_VERSION), LittleEngine::UI::Types::Color::Yellow);
}

#endif
