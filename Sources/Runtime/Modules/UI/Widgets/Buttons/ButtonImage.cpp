/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Buttons/ButtonImage.h"
#include "Modules/UI/Internal/Converter.h"
#include "Modules/UI/ImGui/imgui_internal.h"

LittleEngine::UI::Widgets::Buttons::ButtonImage::ButtonImage(uint32_t p_textureID, const LittleEngine::FVector2 & p_size) :
	textureID{ p_textureID }, size(p_size)
{
}

void LittleEngine::UI::Widgets::Buttons::ButtonImage::_Draw_Impl()
{
	ImVec4 bg = Internal::Converter::ToImVec4(background);
	ImVec4 tn = Internal::Converter::ToImVec4(tint);

	if(isAbsoluteOffset)
	{
		if (ImGui::ImageButton(textureID.raw, Internal::Converter::ToImVec2(offset),Internal::Converter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), -1, bg, tn, disabled ? ImGuiButtonFlags_Disabled : 0))
			ClickedEvent.Invoke();
	}
	else
	{
		if (ImGui::ImageButton(textureID.raw, Internal::Converter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), -1, bg, tn, disabled ? ImGuiButtonFlags_Disabled : 0))
			ClickedEvent.Invoke();
	}
}
