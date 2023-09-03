/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Plugins/ContextualMenu.h"

void OvUI::Plugins::ContextualMenu::Execute()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void OvUI::Plugins::ContextualMenu::Close()
{
	ImGui::CloseCurrentPopup();
}
