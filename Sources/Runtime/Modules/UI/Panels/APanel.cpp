/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "Modules/UI/Panels/APanel.h"

uint64_t LittleEngine::UI::Panels::APanel::__PANEL_ID_INCREMENT = 0;

LittleEngine::UI::Panels::APanel::APanel()
{
	m_panelID = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void LittleEngine::UI::Panels::APanel::Draw()
{
	if (enabled)
		_Draw_Impl();
}

const std::string & LittleEngine::UI::Panels::APanel::GetPanelID() const
{
	return m_panelID;
}
