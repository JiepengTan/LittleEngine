/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Internal/Converter.h"

OvUI::Widgets::Texts::TextColored::TextColored(const std::string& p_content, const Types::Color& p_color) :
	Text(p_content), color(p_color)
{
}

void OvUI::Widgets::Texts::TextColored::_Draw_Impl()
{
	ImGui::TextColored(Internal::Converter::ToImVec4(color), content.c_str());
}
