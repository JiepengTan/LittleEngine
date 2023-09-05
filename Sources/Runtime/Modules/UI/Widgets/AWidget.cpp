/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/AWidget.h"

uint64_t LittleEngine::UI::Widgets::AWidget::__WIDGET_ID_INCREMENT = 0;

LittleEngine::UI::Widgets::AWidget::AWidget()
{
	m_widgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void LittleEngine::UI::Widgets::AWidget::LinkTo(const AWidget& p_widget)
{
	m_widgetID = p_widget.m_widgetID;
}

void LittleEngine::UI::Widgets::AWidget::Destroy()
{
	m_destroyed = true;
}

bool LittleEngine::UI::Widgets::AWidget::IsDestroyed() const
{
	return m_destroyed;
}

void LittleEngine::UI::Widgets::AWidget::SetParent(Internal::WidgetContainer * p_parent)
{
	m_parent = p_parent;
}

bool LittleEngine::UI::Widgets::AWidget::HasParent() const
{
	return m_parent;
}

LittleEngine::UI::Internal::WidgetContainer * LittleEngine::UI::Widgets::AWidget::GetParent()
{
	return m_parent;
}

void LittleEngine::UI::Widgets::AWidget::Draw()
{
	if (enabled)
	{
		_Draw_Impl();

		if (m_autoExecutePlugins)
			ExecutePlugins();

		if (!lineBreak)
			ImGui::SameLine();
	}
}
