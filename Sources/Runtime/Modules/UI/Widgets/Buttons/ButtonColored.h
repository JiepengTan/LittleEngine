/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"

#include "Core/Maths/FVector2.h"

#include "Core/Base/Color.h"
#include "Modules/UI/Widgets/Buttons/AButton.h"

namespace LittleEngine::UI::Widgets::Buttons
{
	/**
	* Button widget of a single color (Color palette element)
	*/
	class ButtonColored : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_label
		* @param p_color
		* @param p_size
		* @param p_enableAlpha
		*/
		ButtonColored(const std::string& p_label = "", const Color& p_color = {}, const LittleEngine::FVector2& p_size =LittleEngine::FVector2(0.f, 0.f), bool p_enableAlpha = true);

	protected:
		void _Draw_Impl() override;

	public:
		std::string label;
		Color color;
		LittleEngine::FVector2 size;
		bool enableAlpha;
	};
}
