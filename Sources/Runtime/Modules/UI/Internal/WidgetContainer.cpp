/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "algorithm"

#include "Modules/UI/Internal/WidgetContainer.h"

void LittleEngine::UI::Internal::WidgetContainer::RemoveWidget(Widgets::AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::pair<LittleEngine::UI::Widgets::AWidget*, Internal::EMemoryMode>& p_pair)
	{ 
		return p_pair.first == &p_widget;
	});

	if (found != m_widgets.end())
	{
		if (found->second == Internal::EMemoryMode::INTERNAL_MANAGMENT)
			delete found->first;

		m_widgets.erase(found);
	}
}

void LittleEngine::UI::Internal::WidgetContainer::RemoveAllWidgets()
{
	std::for_each(m_widgets.begin(), m_widgets.end(), [](auto& pair)
	{
		if (pair.second == Internal::EMemoryMode::INTERNAL_MANAGMENT)
			delete pair.first;
	});

	m_widgets.clear();
}

void LittleEngine::UI::Internal::WidgetContainer::ConsiderWidget(Widgets::AWidget & p_widget, bool p_manageMemory)
{
	m_widgets.emplace_back(std::make_pair(&p_widget, p_manageMemory ? EMemoryMode::INTERNAL_MANAGMENT : EMemoryMode::EXTERNAL_MANAGMENT));
	p_widget.SetParent(this);
}

void LittleEngine::UI::Internal::WidgetContainer::UnconsiderWidget(Widgets::AWidget & p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::pair<LittleEngine::UI::Widgets::AWidget*, Internal::EMemoryMode>& p_pair)
	{
		return p_pair.first == &p_widget;
	});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(nullptr);
		m_widgets.erase(found);
	}
}

void LittleEngine::UI::Internal::WidgetContainer::CollectGarbages()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::pair<LittleEngine::UI::Widgets::AWidget*, Internal::EMemoryMode>& p_item)
	{
		bool toDestroy = p_item.first && p_item.first->IsDestroyed();

		if (toDestroy && p_item.second == Internal::EMemoryMode::INTERNAL_MANAGMENT)
			delete p_item.first;

		return toDestroy;
	}), m_widgets.end());
}

void LittleEngine::UI::Internal::WidgetContainer::DrawWidgets()
{
	CollectGarbages();
    if (m_reversedDrawOrder)
    {
    	m_tempForUpdate.clear();
    	for (auto element : m_widgets)
    	{
    		m_tempForUpdate.push_back(element);
    	}
        for (auto it = m_tempForUpdate.crbegin(); it != m_tempForUpdate.crend(); ++it)
            it->first->Draw();
    }
    else
    {
    	m_tempForUpdate.clear();
	    for (auto element : m_widgets)
	    {
	    	m_tempForUpdate.push_back(element);
	    }
        for (const auto& widget : m_tempForUpdate)
        {
        	widget.first->Draw();
        }
    }
}

void LittleEngine::UI::Internal::WidgetContainer::ReverseDrawOrder(const bool reversed)
{
    m_reversedDrawOrder = reversed;
}

std::vector<std::pair<LittleEngine::UI::Widgets::AWidget*, LittleEngine::UI::Internal::EMemoryMode>>& LittleEngine::UI::Internal::WidgetContainer::GetWidgets()
{
	return m_widgets;
}
