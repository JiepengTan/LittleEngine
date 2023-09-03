/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Types/Color.h"

namespace OvUI::Widgets::Texts
{
	/**
	* Widget to display text on a panel that can be colored
	*/
	class TextColored : public Text
	{
	public:
		/**
		* Constructor
		* @param p_content
		* @param p_color
		*/
		TextColored(const std::string& p_content = "", const Types::Color& p_color = Types::Color(1.0f, 1.0f, 1.0f, 1.0f));

	public:
		Types::Color color;

	protected:
		virtual void _Draw_Impl() override;
	};
}
