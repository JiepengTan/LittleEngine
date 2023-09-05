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
	* Panel that display information about the driver (hardware and software)
	*/
	class DriverInfo : public LittleEngine::UI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		* @param p_window
		*/
		DriverInfo(LittleEngine::Rendering::Core::Renderer& p_renderer, LittleEngine::Windowing::Window& p_window);
	};
}

#endif
