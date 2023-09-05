/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Layout/Spacing.h"

#include "../Editor/Panels/Toolbar.h"
#include "../Editor/Core/EditorActions.h"

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"

LittleEditor::Panels::Toolbar::Toolbar
(
	const std::string& p_title,
	bool p_opened,
	const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	std::string iconFolder = ":Textures/Icons/";

	auto& textureManager = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::TextureManager>();

	m_playButton	= &CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Play")->id, LittleEngine::FVector2{ 20, 20 });
	m_pauseButton	= &CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Pause")->id, LittleEngine::FVector2{ 20, 20 });
	m_stopButton	= &CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Stop")->id, LittleEngine::FVector2{ 20, 20 });
	m_nextButton	= &CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Next")->id, LittleEngine::FVector2{ 20, 20 });

	CreateWidget<LittleEngine::UI::Widgets::Layout::Spacing>(0).lineBreak = false;
	auto& refreshButton	= CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Refresh")->id, LittleEngine::FVector2{ 20, 20 });

	m_playButton->lineBreak		= false;
	m_pauseButton->lineBreak	= false;
	m_stopButton->lineBreak		= false;
	m_nextButton->lineBreak		= false;
	refreshButton.lineBreak		= false;

	m_playButton->ClickedEvent	+= EDITOR_BIND(StartPlaying);
	m_pauseButton->ClickedEvent	+= EDITOR_BIND(PauseGame);
	m_stopButton->ClickedEvent	+= EDITOR_BIND(StopPlaying);
	m_nextButton->ClickedEvent	+= EDITOR_BIND(NextFrame);
	refreshButton.ClickedEvent	+= EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](LittleEditor::Core::EditorActions::EEditorMode p_newMode)
	{
		auto enable = [](LittleEngine::UI::Widgets::Buttons::ButtonImage* p_button, bool p_enable)
		{
			p_button->disabled = !p_enable;
			p_button->tint = p_enable ? LittleEngine::UI::Types::Color{ 1.0f, 1.0f, 1.0f, 1.0f} : LittleEngine::UI::Types::Color{1.0f, 1.0f, 1.0f, 0.15f};
		};

		switch (p_newMode)
		{
		case LittleEditor::Core::EditorActions::EEditorMode::EDIT:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, false);
			enable(m_nextButton, false);
			break;
		case LittleEditor::Core::EditorActions::EEditorMode::PLAY:
			enable(m_playButton, false);
			enable(m_pauseButton, true);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case LittleEditor::Core::EditorActions::EEditorMode::PAUSE:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case LittleEditor::Core::EditorActions::EEditorMode::FRAME_BY_FRAME:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		}
	};

	EDITOR_EXEC(SetEditorMode(LittleEditor::Core::EditorActions::EEditorMode::EDIT));
}

void LittleEditor::Panels::Toolbar::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

	LittleEngine::UI::Panels::PanelWindow::_Draw_Impl();

	ImGui::PopStyleVar();
}
