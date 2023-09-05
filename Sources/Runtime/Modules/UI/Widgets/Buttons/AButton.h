/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>

#include "Core/Tools/Eventing/Event.h"

#include "Modules/UI/Widgets/AWidget.h"

namespace LittleEngine::UI::Widgets::Buttons
{
	/**
	* Base class for any button widget
	*/
	class AButton : public AWidget
	{
	protected:
		void _Draw_Impl() override = 0;

	public:
		LittleEngine::Eventing::Event<> ClickedEvent;
	};
}
