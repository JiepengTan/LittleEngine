/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"

#include "Core/Maths/FVector2.h"

#include "Modules/UI/Widgets/Buttons/AButton.h"
#include "Core/Base/Color.h"

namespace LittleEngine::UI::Widgets::Buttons
{
	/**
	* Simple button widget
	*/
	class Button : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_label
		* @param p_size
		* @param p_disabled
		*/
		Button(const std::string& p_label = "", const LittleEngine::FVector2& p_size = LittleEngine::FVector2(0.f, 0.f), bool p_disabled = false);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		LittleEngine::FVector2 size;
		bool disabled = false;

		Color idleBackgroundColor;
		Color hoveredBackgroundColor;
		Color clickedBackgroundColor;

		Color textColor;
	};
}
