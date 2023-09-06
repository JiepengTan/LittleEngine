/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Time/Clock.h"

#include "../Editor/Core/EditorApp.h"

LittleEngine::Editor::Core::EditorApp::EditorApp(const std::string& p_projectPath, const std::string& p_projectName) :
	m_context(p_projectPath, p_projectName),
	m_editor(m_context)
{
	EDITOR_EXEC(LoadLastScene());
}

LittleEngine::Editor::Core::EditorApp::~EditorApp()
{
}

void LittleEngine::Editor::Core::EditorApp::Run()
{
	LittleEngine::Time::Clock clock;

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool LittleEngine::Editor::Core::EditorApp::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
