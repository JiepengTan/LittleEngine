/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#ifdef _DEBUG

#include "Modules/Rendering/Core/Renderer.h"
#include "Platform/Windowing/Window.h"

#include "Core/Analytics/Profiling/Profiler.h"
#include "Modules/UI/Panels/PanelUndecorated.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Buttons/Button.h"

namespace LittleGame::Debug
{
	/**
	* Panel that display profiling information
	*/
	class GameProfiler : public LittleEngine::UI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_window
		* @param p_frequency
		*/
		GameProfiler(LittleEngine::Windowing::Window& p_window, float p_frequency);

		/**
		* Update the data
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);
	

	private:
		LittleEngine::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(LittleEngine::Analytics::Profiling::ProfilerReport::Action& p_action);

	private:

		float m_frequency;
		float m_timer = 0.f;

		LittleEngine::Analytics::Profiling::Profiler m_profiler;

		LittleEngine::Windowing::Window& m_window;
		LittleEngine::UI::Widgets::AWidget* m_separator;
		LittleEngine::UI::Widgets::Texts::TextColored* m_elapsedFramesText;
		LittleEngine::UI::Widgets::Texts::TextColored* m_elapsedTimeText;
		LittleEngine::UI::Widgets::Layout::Group* m_actionList;
	};
}

#endif
