/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Panels/APanelTransformable.h"
#include "Modules/UI/Internal/Converter.h"

LittleEngine::UI::Panels::APanelTransformable::APanelTransformable
(
	const LittleEngine::FVector2& p_defaultPosition,
	const LittleEngine::FVector2& p_defaultSize,
	Settings::EHorizontalAlignment p_defaultHorizontalAlignment,
	Settings::EVerticalAlignment p_defaultVerticalAlignment,
	bool p_ignoreConfigFile
) :
	m_defaultPosition(p_defaultPosition),
	m_defaultSize(p_defaultSize),
	m_defaultHorizontalAlignment(p_defaultHorizontalAlignment),
	m_defaultVerticalAlignment(p_defaultVerticalAlignment),
	m_ignoreConfigFile(p_ignoreConfigFile)
{
}

void LittleEngine::UI::Panels::APanelTransformable::SetPosition(const LittleEngine::FVector2& p_position)
{
	m_position = p_position;
	m_positionChanged = true;
}

void LittleEngine::UI::Panels::APanelTransformable::SetSize(const LittleEngine::FVector2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}

void LittleEngine::UI::Panels::APanelTransformable::SetAlignment(Settings::EHorizontalAlignment p_horizontalAlignment, Settings::EVerticalAlignment p_verticalAligment)
{
	m_horizontalAlignment = p_horizontalAlignment;
	m_verticalAlignment = p_verticalAligment;
	m_alignmentChanged = true;
}

const LittleEngine::FVector2 & LittleEngine::UI::Panels::APanelTransformable::GetPosition() const
{
	return m_position;
}

const LittleEngine::FVector2 & LittleEngine::UI::Panels::APanelTransformable::GetSize() const
{
	return m_size;
}

LittleEngine::UI::Settings::EHorizontalAlignment LittleEngine::UI::Panels::APanelTransformable::GetHorizontalAlignment() const
{
	return m_horizontalAlignment;
}

LittleEngine::UI::Settings::EVerticalAlignment LittleEngine::UI::Panels::APanelTransformable::GetVerticalAlignment() const
{
	return m_verticalAlignment;
}

void LittleEngine::UI::Panels::APanelTransformable::UpdatePosition()
{
	if (m_defaultPosition.x != -1.f && m_defaultPosition.y != 1.f)
	{
		LittleEngine::FVector2 offsettedDefaultPos = m_defaultPosition + CalculatePositionAlignmentOffset(true);
		ImGui::SetWindowPos(Internal::Converter::ToImVec2(offsettedDefaultPos), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	}

	if (m_positionChanged || m_alignmentChanged)
	{
		LittleEngine::FVector2 offset = CalculatePositionAlignmentOffset(false);
		LittleEngine::FVector2 offsettedPos(m_position.x + offset.x, m_position.y + offset.y);
		ImGui::SetWindowPos(Internal::Converter::ToImVec2(offsettedPos), ImGuiCond_Always);
		m_positionChanged = false;
		m_alignmentChanged = false;
	}
}

void LittleEngine::UI::Panels::APanelTransformable::UpdateSize()
{
	/*
	if (m_defaultSize.x != -1.f && m_defaultSize.y != 1.f)
		ImGui::SetWindowSize(Internal::Converter::ToImVec2(m_defaultSize), m_ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
	*/
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(Internal::Converter::ToImVec2(m_size), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

void LittleEngine::UI::Panels::APanelTransformable::CopyImGuiPosition()
{
	m_position = Internal::Converter::ToFVector2(ImGui::GetWindowPos());
}

void LittleEngine::UI::Panels::APanelTransformable::CopyImGuiSize()
{
	m_size = Internal::Converter::ToFVector2(ImGui::GetWindowSize());
}

void LittleEngine::UI::Panels::APanelTransformable::Update()
{
	if (!m_firstFrame)
	{
		if (!autoSize)
			UpdateSize();
		CopyImGuiSize();

		UpdatePosition();
		CopyImGuiPosition();
	}

	m_firstFrame = false;
}

LittleEngine::FVector2 LittleEngine::UI::Panels::APanelTransformable::CalculatePositionAlignmentOffset(bool p_default)
{
	LittleEngine::FVector2 result(0.0f, 0.0f);

	switch (p_default ? m_defaultHorizontalAlignment : m_horizontalAlignment)
	{
	case LittleEngine::UI::Settings::EHorizontalAlignment::CENTER:
		result.x -= m_size.x / 2.0f;
		break;
	case LittleEngine::UI::Settings::EHorizontalAlignment::RIGHT:
		result.x -= m_size.x;
		break;
	}

	switch (p_default ? m_defaultVerticalAlignment : m_verticalAlignment)
	{
	case LittleEngine::UI::Settings::EVerticalAlignment::MIDDLE:
		result.y -= m_size.y / 2.0f;
		break;
	case LittleEngine::UI::Settings::EVerticalAlignment::BOTTOM:
		result.y -= m_size.y;
		break;
	}

	return result;
}
