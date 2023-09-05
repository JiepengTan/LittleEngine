/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Panels/PanelUndecorated.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"

#include "Platform/Windowing/Window.h"

namespace LittleGame::Utils
{
	/**
	* Panel that display the framerate
	*/
	class FPSCounter : public LittleEngine::UI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_window
		*/
		FPSCounter(LittleEngine::Windowing::Window& p_window);

		/**
		* Update the data
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		LittleEngine::UI::Widgets::Texts::TextColored m_text;

		LittleEngine::Windowing::Window& m_window;
		float m_elapsed = 0.0f;
		uint32_t m_frames = 0;
	};
}
