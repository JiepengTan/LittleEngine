/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Tools/Eventing/Event.h"

#include "Modules/UI/Widgets/DataWidget.h"
#include "Core/Base/Color.h"

namespace LittleEngine::UI::Widgets::Selection
{
	/**
	* Widget that allow the selection of a color with a color picker
	*/
	class ColorPicker : public DataWidget<Color>
	{
	public:
		/**
		* Constructor
		* @param p_enableAlpha
		* @param p_defaultColor
		*/
		ColorPicker(bool p_enableAlpha = false, const Color& p_defaultColor = {});

	protected:
		void _Draw_Impl() override;

	public:
		bool enableAlpha;
		Color color;
		LittleEngine::Eventing::Event<Color&> ColorChangedEvent;
	};
}
