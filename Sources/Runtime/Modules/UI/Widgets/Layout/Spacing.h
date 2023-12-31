/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Widgets/AWidget.h"

namespace LittleEngine::UI::Widgets::Layout
{
	/**
	* Widget that adds a space to the panel line
	*/
	class Spacing : public AWidget
	{
	public:
		/**
		* Constructor
		* @param p_spaces
		*/
		Spacing(uint16_t p_spaces = 1);

	protected:
		void _Draw_Impl() override;

	public:
		uint16_t spaces = 1;
	};
}
