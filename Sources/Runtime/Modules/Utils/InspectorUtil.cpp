#include "InspectorUtil.h"

#include "ImGui/imgui_internal.h"
#include "Modules/UI/Internal/Converter.h"
using namespace LittleEngine::UI::Internal;
namespace LittleEngine
{
    uint64_t InspectorUtil::s_unique_id = 0;
    std::string InspectorUtil::s_unique_id_str = "";
    std::string InspectorUtil::s_startWidgetId = "";
    
    void InspectorUtil::OnComponentStart(std::string& startWidgetId)
    {
        s_startWidgetId = startWidgetId;
        s_unique_id = 0;
    }
    
    void InspectorUtil::CreateTitle(const std::string& p_name)
    {
        
    }

    const char* InspectorUtil::GetUniqueName(const std::string& p_name)
    {
        s_unique_id_str = p_name+ s_startWidgetId+"1000"+std::to_string(s_unique_id++);
        return s_unique_id_str.c_str();  
    }


    void InspectorUtil::DrawDefault(Component* component)
    {
        if(DrawButton("Hello world"))
        {
            LOG_INFO("asdafsfff   DrawDefault");
        }
    }

    bool InspectorUtil::DrawButton(const std::string& p_name,FVector2 p_size, bool p_disabled )
    {
	    auto& style = ImGui::GetStyle();
        auto idleBackgroundColor		= Converter::ToColor(style.Colors[ImGuiCol_Button]);
        auto hoveredBackgroundColor	    = Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
        auto clickedBackgroundColor	    = Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
        auto textColor				    = Converter::ToColor(style.Colors[ImGuiCol_Text]);

        auto defaultIdleColor		= style.Colors[ImGuiCol_Button];
        auto defaultHoveredColor	= style.Colors[ImGuiCol_ButtonHovered];
        auto defaultClickedColor	= style.Colors[ImGuiCol_ButtonActive];
        auto defaultTextColor		= style.Colors[ImGuiCol_Text];

        style.Colors[ImGuiCol_Button]			= Converter::ToImVec4(idleBackgroundColor);
        style.Colors[ImGuiCol_ButtonHovered]	= Converter::ToImVec4(hoveredBackgroundColor);
        style.Colors[ImGuiCol_ButtonActive]		= Converter::ToImVec4(clickedBackgroundColor);
        style.Colors[ImGuiCol_Text]				= Converter::ToImVec4(textColor);

        bool clicked =ImGui::Button(GetUniqueName(p_name), Converter::ToImVec2(p_size));
        
        style.Colors[ImGuiCol_Button]			= defaultIdleColor;
        style.Colors[ImGuiCol_ButtonHovered]	= defaultHoveredColor;
        style.Colors[ImGuiCol_ButtonActive]		= defaultClickedColor;
        style.Colors[ImGuiCol_Text]				= defaultTextColor;
        return clicked;
    }
}
