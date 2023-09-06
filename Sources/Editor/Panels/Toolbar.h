/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Widgets/Buttons/ButtonImage.h"
#include "Modules/UI/Panels/PanelWindow.h"

namespace LittleEngine::Editor::Panels
{
	class Toolbar : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Toolbar
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Custom implementation of the draw method
		*/
		void _Draw_Impl() override;

	private:
		LittleEngine::UI::Widgets::Buttons::ButtonImage* m_playButton;
		LittleEngine::UI::Widgets::Buttons::ButtonImage* m_pauseButton;
		LittleEngine::UI::Widgets::Buttons::ButtonImage* m_stopButton;
		LittleEngine::UI::Widgets::Buttons::ButtonImage* m_nextButton;
	};
}
