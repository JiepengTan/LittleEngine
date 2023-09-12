#include "InspectorProxy.h"

#include "Modules/Utils/InspectorUtil.h"

namespace LittleEngine::UI::Widgets::Layout
{
    void InspectorProxy::_Draw_Impl()
    {
        InspectorUtil::OnComponentStart(this->m_widgetID);
        if(m_callback != nullptr)
        {
            m_callback();
        }
    }
}
