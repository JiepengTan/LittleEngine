/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Layout/Dummy.h"
#include "Modules/UI/Internal/Converter.h"

LittleEngine::UI::Widgets::Layout::Dummy::Dummy(const LittleEngine::FVector2& p_size) : size(p_size)
{
}

void LittleEngine::UI::Widgets::Layout::Dummy::_Draw_Impl()
{
	ImGui::Dummy(Internal::Converter::ToImVec2(size));
}
