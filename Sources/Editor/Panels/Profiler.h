/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Analytics/Profiling/Profiler.h"

#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Widgets/Selection/CheckBox.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/UI/Widgets/Buttons/Button.h"

namespace LittleEditor::Panels
{
	class Profiler : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_frequency
		*/
		Profiler
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings,
			float p_frequency
		);

		/**
		* Update profiling information
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Enable or disable the profiler
		* @param p_value
		* @param p_disableLog
		*/
		void Enable(bool p_value, bool p_disableLog = false);

	private:
		LittleEngine::UI::Types::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(LittleEngine::Analytics::Profiling::ProfilerReport::Action& p_action);

	private:
		enum class EProfilingMode
		{
			DEFAULT,
			CAPTURE
		};

		float m_frequency;
		float m_timer = 0.f;
		float m_fpsTimer = 0.f;
		EProfilingMode m_profilingMode = EProfilingMode::DEFAULT;

		LittleEngine::Analytics::Profiling::Profiler m_profiler;

		LittleEngine::UI::Widgets::AWidget* m_separator;
		LittleEngine::UI::Widgets::Buttons::Button* m_captureResumeButton;
		LittleEngine::UI::Widgets::Texts::TextColored* m_fpsText;
		LittleEngine::UI::Widgets::Texts::TextColored* m_elapsedFramesText;
		LittleEngine::UI::Widgets::Texts::TextColored* m_elapsedTimeText;
		LittleEngine::UI::Widgets::Layout::Columns<5>* m_actionList;
	};
}
