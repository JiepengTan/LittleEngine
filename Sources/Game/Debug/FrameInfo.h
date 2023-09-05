/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#ifdef _DEBUG

#include "Modules/Rendering/Core/Renderer.h"
#include "Platform/Windowing/Window.h"

#include "Modules/UI/Panels/PanelUndecorated.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"

namespace LittleGame::Debug
{
	/**
	* Panel that display information about the frame
	*/
	class FrameInfo : public LittleEngine::UI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		* @param p_window
		*/
		FrameInfo(LittleEngine::Rendering::Core::Renderer& p_renderer, LittleEngine::Windowing::Window& p_window);

		/**
		* Update the data
		* @parma p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		LittleEngine::Rendering::Core::Renderer&	m_renderer;
		LittleEngine::Windowing::Window&			m_window;

		LittleEngine::UI::Widgets::Texts::TextColored* m_frameInfo[3];
	};
}

#endif
