/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Plots/PlotLines.h"
#include "Modules/UI/Widgets/Plots/PlotHistogram.h"

namespace LittleEngine::Analytics::Hardware { class HardwareInfo; }

namespace LittleEngine::Editor::Panels
{
	class HardwareInfo : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_logFrequency
		* @param p_maxElements
		* @param p_displayMode
		*/
		HardwareInfo
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings,
			float p_logFrequency,
			size_t p_maxElements
		);

		/**
		* Destructor
		*/
		~HardwareInfo();

		/**
		* Update hardware info
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		float p_updateTimer = 0.f;
		float m_logFrequency;
		size_t m_maxElements;
		LittleEngine::UI::Widgets::Plots::APlot* m_cpuUsage;
		LittleEngine::UI::Widgets::Plots::APlot* m_gpuUsage;
		LittleEngine::UI::Widgets::Plots::APlot* m_ramUsage;
		LittleEngine::Analytics::Hardware::HardwareInfo* m_hardwareInfo;
	};
}
