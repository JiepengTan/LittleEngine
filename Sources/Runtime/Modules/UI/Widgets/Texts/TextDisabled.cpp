/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/TextDisabled.h"

OvUI::Widgets::Texts::TextDisabled::TextDisabled(const std::string & p_content) :
	Text(p_content)
{
}

void OvUI::Widgets::Texts::TextDisabled::_Draw_Impl()
{
	ImGui::TextDisabled(content.c_str());
}
