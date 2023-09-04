/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#ifdef _DEBUG

#include "Resource/Rendering/Core/Renderer.h"
#include "Platform/Windowing/Window.h"

#include "Modules/UI/Panels/PanelUndecorated.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"

namespace OvGame::Debug
{
	/**
	* Panel that display information about the frame
	*/
	class FrameInfo : public OvUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		* @param p_window
		*/
		FrameInfo(OvRendering::Core::Renderer& p_renderer, OvWindowing::Window& p_window);

		/**
		* Update the data
		* @parma p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		OvRendering::Core::Renderer&	m_renderer;
		OvWindowing::Window&			m_window;

		OvUI::Widgets::Texts::TextColored* m_frameInfo[3];
	};
}

#endif
