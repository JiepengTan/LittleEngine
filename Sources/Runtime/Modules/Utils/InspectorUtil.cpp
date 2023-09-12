#include "InspectorUtil.h"

#include "../../../Tools/MetaParser/parser/meta/meta_data_config.h"
#include "Core/Reflection/MetaDefine.h"
#include "ImGui/imgui_internal.h"
#include "Modules/Framework/ECS/Component.h"
#include "Modules/UI/Internal/Converter.h"
#include "Core/Reflection/TypeUtil.h"
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
        if(component == nullptr) return;
        auto type = component->GetType();
        if(type->HasMeta(NativeProperty::CustomerEditor))
        {
            if(DrawButton("Hello world"))
            {
                LOG_INFO("asdafsfff   DrawDefault");
            }
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

    bool InspectorUtil::DrawBoolean(const std::string& p_name, bool& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawVec2(const std::string& p_name, FVector2& p_data, float p_step, float p_min, float p_max)
    {
        return false;
    }

    bool InspectorUtil::DrawVec3(const std::string& p_name, FVector3& p_data, float p_step, float p_min, float p_max)
    {
        return false;
    }

    bool InspectorUtil::DrawVec4(const std::string& p_name, FVector4& p_data, float p_step, float p_min, float p_max)
    {
        return false;
    }

    bool InspectorUtil::DrawQuat(const std::string& p_name, FQuaternion& p_data, float p_step, float p_min, float p_max)
    {
        return false;
    }

    bool InspectorUtil::DrawString(const std::string& p_name, std::string& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawColor(const std::string& p_name, Color& p_color, bool p_hasAlpha)
    {
        return false;
    }

    bool InspectorUtil::DrawMesh(const std::string& p_name, Model*& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawTexture(const std::string& p_name, Texture*& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawTexture(const std::string& p_name, Texture*& p_data, std::string& guid)
    {
        return false;
    }

    bool InspectorUtil::DrawTexture(const std::string& p_name, TextureResPtr& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawShader(const std::string& p_name, Shader*& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawMaterial(const std::string& p_name, Material*& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawSound(const std::string& p_name, Sound*& p_data)
    {
        return false;
    }

    bool InspectorUtil::DrawAsset(const std::string& p_name, std::string& p_data)
    {
        return false;
    }
}
