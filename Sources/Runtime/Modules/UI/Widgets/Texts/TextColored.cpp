/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Internal/Converter.h"

LittleEngine::UI::Widgets::Texts::TextColored::TextColored(const std::string& p_content, const Color& p_color) :
	Text(p_content), color(p_color)
{
}

void LittleEngine::UI::Widgets::Texts::TextColored::_Draw_Impl()
{
	ImGui::TextColored(Internal::Converter::ToImVec4(color), content.c_str());
}
