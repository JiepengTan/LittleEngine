/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Selection/ColorEdit.h"

LittleEngine::UI::Widgets::Selection::ColorEdit::ColorEdit(bool p_enableAlpha, const Color & p_defaultColor) :
	DataWidget<Color>(color), enableAlpha(p_enableAlpha), color(p_defaultColor)
{
}

void LittleEngine::UI::Widgets::Selection::ColorEdit::_Draw_Impl()
{
	int flags = !enableAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
	bool valueChanged = false;

	if (enableAlpha)
		valueChanged = ImGui::ColorEdit4(m_widgetID.c_str(), &color.r, flags);
	else
		valueChanged =ImGui::ColorEdit3(m_widgetID.c_str(), &color.r, flags);

	if (valueChanged)
	{
		ColorChangedEvent.Invoke(color);
		this->NotifyChange();
	}
}
