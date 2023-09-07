/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Buttons/ButtonColored.h"
#include "Modules/UI/Internal/Converter.h"

LittleEngine::UI::Widgets::Buttons::ButtonColored::ButtonColored(const std::string & p_label, const Color& p_color, const LittleEngine::FVector2& p_size, bool p_enableAlpha) :
	label(p_label), color(p_color), size(p_size), enableAlpha(p_enableAlpha)
{
}

void LittleEngine::UI::Widgets::Buttons::ButtonColored::_Draw_Impl()
{
	ImVec4 imColor = Internal::Converter::ToImVec4(color);

	if (ImGui::ColorButton((label + m_widgetID).c_str(), imColor, !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0, Internal::Converter::ToImVec2(size)))
		ClickedEvent.Invoke();

	color = Internal::Converter::ToColor(imColor);
}
