/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "iostream"

#include "Platform/Windowing/Inputs/InputManager.h"

LittleEngine::Windowing::Inputs::InputManager::InputManager(Window& p_window) : m_window(p_window)
{
	m_keyPressedListener = m_window.KeyPressedEvent.AddListener(std::bind(&InputManager::OnKeyPressed, this, std::placeholders::_1));
	m_keyReleasedListener = m_window.KeyReleasedEvent.AddListener(std::bind(&InputManager::OnKeyReleased, this, std::placeholders::_1));
	m_mouseButtonPressedListener = m_window.MouseButtonPressedEvent.AddListener(std::bind(&InputManager::OnMouseButtonPressed, this, std::placeholders::_1));
	m_mouseButtonReleasedListener = m_window.MouseButtonReleasedEvent.AddListener(std::bind(&InputManager::OnMouseButtonReleased, this, std::placeholders::_1));
}

LittleEngine::Windowing::Inputs::InputManager::~InputManager()
{
	m_window.KeyPressedEvent.RemoveListener(m_keyPressedListener);
	m_window.KeyReleasedEvent.RemoveListener(m_keyReleasedListener);
	m_window.MouseButtonPressedEvent.RemoveListener(m_mouseButtonPressedListener);
	m_window.MouseButtonReleasedEvent.RemoveListener(m_mouseButtonReleasedListener);
}

LittleEngine::Windowing::Inputs::EKeyState LittleEngine::Windowing::Inputs::InputManager::GetKeyState(EKey p_key) const
{
	switch (glfwGetKey(m_window.GetGlfwWindow(), static_cast<int>(p_key)))
	{
		case GLFW_PRESS:	return EKeyState::KEY_DOWN;
		case GLFW_RELEASE:	return EKeyState::KEY_UP;
	}

	return EKeyState::KEY_UP;
}

LittleEngine::Windowing::Inputs::EMouseButtonState LittleEngine::Windowing::Inputs::InputManager::GetMouseButtonState(EMouseButton p_button) const
{
	switch (glfwGetMouseButton(m_window.GetGlfwWindow(), static_cast<int>(p_button)))
	{
		case GLFW_PRESS:	return EMouseButtonState::MOUSE_DOWN;
		case GLFW_RELEASE:	return EMouseButtonState::MOUSE_UP;
	}

	return EMouseButtonState::MOUSE_UP;
}

bool LittleEngine::Windowing::Inputs::InputManager::IsKeyPressed(EKey p_key) const
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::KEY_DOWN;
}

bool LittleEngine::Windowing::Inputs::InputManager::IsKeyReleased(EKey p_key) const
{
	return m_keyEvents.find(p_key) != m_keyEvents.end() && m_keyEvents.at(p_key) == EKeyState::KEY_UP;
}

bool LittleEngine::Windowing::Inputs::InputManager::IsMouseButtonPressed(EMouseButton p_button) const
{
	return m_mouseButtonEvents.find(p_button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == EMouseButtonState::MOUSE_DOWN;
}

bool LittleEngine::Windowing::Inputs::InputManager::IsMouseButtonReleased(EMouseButton p_button) const
{
	return m_mouseButtonEvents.find(p_button) != m_mouseButtonEvents.end() && m_mouseButtonEvents.at(p_button) == EMouseButtonState::MOUSE_UP;
}

std::pair<double, double> LittleEngine::Windowing::Inputs::InputManager::GetMousePosition() const
{
	std::pair<double, double> result;
	glfwGetCursorPos(m_window.GetGlfwWindow(), &result.first, &result.second);
	return result;
}

void LittleEngine::Windowing::Inputs::InputManager::ClearEvents()
{
	m_keyEvents.clear();
	m_mouseButtonEvents.clear();
}

void LittleEngine::Windowing::Inputs::InputManager::OnKeyPressed(int p_key)
{
	m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::KEY_DOWN;
}

void LittleEngine::Windowing::Inputs::InputManager::OnKeyReleased(int p_key)
{
	m_keyEvents[static_cast<EKey>(p_key)] = EKeyState::KEY_UP;
}

void LittleEngine::Windowing::Inputs::InputManager::OnMouseButtonPressed(int p_button)
{
	m_mouseButtonEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_DOWN;
}

void LittleEngine::Windowing::Inputs::InputManager::OnMouseButtonReleased(int p_button)
{
	m_mouseButtonEvents[static_cast<EMouseButton>(p_button)] = EMouseButtonState::MOUSE_UP;
}
