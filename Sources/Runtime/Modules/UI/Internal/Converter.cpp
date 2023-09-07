/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Internal/Converter.h"

ImVec4 LittleEngine::UI::Internal::Converter::ToImVec4(const Color & p_value)
{
	return ImVec4(p_value.r, p_value.g, p_value.b, p_value.a);
}

LittleEngine::Color LittleEngine::UI::Internal::Converter::ToColor(const ImVec4 & p_value)
{
	return Color(p_value.x, p_value.y, p_value.z, p_value.w);
}

ImVec2 LittleEngine::UI::Internal::Converter::ToImVec2(const LittleEngine::FVector2 & p_value)
{
	return ImVec2(p_value.x, p_value.y);
}

LittleEngine::FVector2 LittleEngine::UI::Internal::Converter::ToFVector2(const ImVec2 & p_value)
{
	return LittleEngine::FVector2(p_value.x, p_value.y);
}
