/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Layout/GroupCollapsable.h"
#include "ImGui/imgui_internal.h"

LittleEngine::UI::Widgets::Layout::GroupCollapsable::GroupCollapsable(const std::string & p_name) :
	name(p_name)
{
}

void LittleEngine::UI::Widgets::Layout::GroupCollapsable::_Draw_Impl()
{
	bool previouslyOpened = opened;

	if (ImGui::CollapsingHeader(name.c_str(), closable ? &opened : nullptr))
		Group::_Draw_Impl();

	if (opened != previouslyOpened)
	{
		if (opened)
			OpenEvent.Invoke();
		else
			CloseEvent.Invoke();
	}
}
