#pragma once
#include <functional>
#include "Modules/UI/Widgets/AWidget.h"
#include "Modules/UI/Internal/WidgetContainer.h"

namespace LittleEngine::UI::Widgets::Layout
{
    class InspectorProxy : public AWidget, public Internal::WidgetContainer
    {
    public:
        InspectorProxy()=default;
        void SetDrawCallback(std::function<void()> callback)
        {
            m_callback = callback;
        }
    protected:
        virtual void _Draw_Impl() override;
        std::function<void()> m_callback;
    };
}
