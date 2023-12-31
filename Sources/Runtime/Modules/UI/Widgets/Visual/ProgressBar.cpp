/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Visual/ProgressBar.h"
#include "Modules/UI/Internal/Converter.h"

LittleEngine::UI::Widgets::Visual::ProgressBar::ProgressBar(float p_fraction, const LittleEngine::FVector2 & p_size, const std::string & p_overlay) :
	fraction(p_fraction), size(p_size), overlay(p_overlay)
{
}

void LittleEngine::UI::Widgets::Visual::ProgressBar::_Draw_Impl()
{
	ImGui::ProgressBar(fraction, Internal::Converter::ToImVec2(size), !overlay.empty() ? overlay.c_str() : nullptr);
}
