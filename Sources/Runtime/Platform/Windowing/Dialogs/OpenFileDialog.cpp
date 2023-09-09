/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Platform/Windowing/Dialogs/OpenFileDialog.h"

#include "Windows.h"

LittleEngine::Windowing::Dialogs::OpenFileDialog::OpenFileDialog(const std::string & p_dialogTitle) : FileDialog(GetOpenFileNameA, p_dialogTitle)
{
}

void LittleEngine::Windowing::Dialogs::OpenFileDialog::AddFileType(const std::string & p_label, const std::string & p_filter)
{
	m_filter += p_label + '\0' + p_filter + '\0';
}
