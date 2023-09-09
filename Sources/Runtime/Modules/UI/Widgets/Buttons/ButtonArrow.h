/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"

#include "Modules/UI/Widgets/Buttons/AButton.h"

namespace LittleEngine::UI::Widgets::Buttons
{
	/**
	* Button widget with an arrow image
	*/
	class ButtonArrow : public AButton
	{
	public:
		/**
		* Create the button
		* @param p_direction
		*/
		ButtonArrow(ImGuiDir p_direction = ImGuiDir_None);

	protected:
		void _Draw_Impl() override;

	public:
		ImGuiDir direction;
	};
}
