/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Core/Tools/Eventing/Event.h"

#include "Modules/UI/Panels/APanel.h"
#include "Modules/UI/Widgets/Menu/MenuList.h"

namespace LittleEngine::UI::Panels
{
	/**
	* A simple panel that will be displayed on the top side of the canvas
	*/
	class PanelMenuBar : public APanel
	{
	protected:
		void _Draw_Impl() override;
	};
}
