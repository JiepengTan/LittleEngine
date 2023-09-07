/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>

#include "Modules/UI/Widgets/Buttons/AButton.h"
#include "Core/Base/Color.h"

namespace LittleEngine::UI::Widgets::Buttons
{
	/**
	* Small button widget
	*/
	class ButtonSmall : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_label
		*/
		ButtonSmall(const std::string& p_label = "");

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;

		Color idleBackgroundColor;
		Color hoveredBackgroundColor;
		Color clickedBackgroundColor;

		Color textColor;
	};
}
