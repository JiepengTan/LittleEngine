/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Time/Clock.h"

#include "../Editor/Core/EditorApp.h"

LittleEditor::Core::EditorApp::EditorApp(const std::string& p_projectPath, const std::string& p_projectName) :
	m_context(p_projectPath, p_projectName),
	m_editor(m_context)
{
	EDITOR_EXEC(LoadLastScene());
}

LittleEditor::Core::EditorApp::~EditorApp()
{
}

void LittleEditor::Core::EditorApp::Run()
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

bool LittleEditor::Core::EditorApp::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
