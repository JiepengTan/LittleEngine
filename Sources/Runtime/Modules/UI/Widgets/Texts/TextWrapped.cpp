/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/TextWrapped.h"

LittleEngine::UI::Widgets::Texts::TextWrapped::TextWrapped(const std::string & p_content) :
	Text(p_content)
{
}

void LittleEngine::UI::Widgets::Texts::TextWrapped::_Draw_Impl()
{
	ImGui::TextWrapped(content.c_str());
}
