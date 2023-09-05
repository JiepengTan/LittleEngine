/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Plugins/ContextualMenu.h"

void LittleEngine::UI::Plugins::ContextualMenu::Execute()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void LittleEngine::UI::Plugins::ContextualMenu::Close()
{
	ImGui::CloseCurrentPopup();
}
