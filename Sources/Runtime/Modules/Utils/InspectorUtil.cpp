#include "InspectorUtil.h"

#include "Core/Reflection/MetaDefine.h"
#include "ImGui/imgui_internal.h"
#include "Modules/Framework/ECS/Component.h"
#include "Modules/UI/Internal/Converter.h"
#include "Core/Reflection/TypeUtil.h"
using namespace LittleEngine::UI::Internal;

namespace LittleEngine
{
    class BuildInTypeName
    {
        const std::string Byte = "int";
        const std::string SByte = "int";
        const std::string Int = "int";
        const std::string UInt = "int";
        const std::string Long = "int";
        const std::string ULong = "int";
        const std::string Float = "int";
        const std::string Double = "int";
        const std::string Bool = "int";
        const std::string String = "int";
        const std::string Color = "int";
        const std::string FVector2 = "int";
        const std::string FVector3 = "int";
        const std::string FVector4 = "int";
        const std::string FQuaternion = "int";
        const std::string TextureResPtr = "int";
        const std::string ModelResPtr = "int";
        const std::string SoundResPtr = "int";
        const std::string MaterialResPtr = "int";
        const std::string ShaderResPtr = "int";
    };

    uint64_t InspectorUtil::s_unique_id = 0;
    std::string InspectorUtil::s_unique_id_str = "";
    std::string InspectorUtil::s_startWidgetId = "";
    int InspectorUtil::s_globalIndent = 0;
    int InspectorUtil::s_indentSpaceFactor = 20;
    std::unordered_map<std::string, InspectorUtil::TypeDrawFunc> InspectorUtil::s_type2DrawFunction;

    void InspectorUtil::OnComponentStart(std::string& startWidgetId)
    {
        s_startWidgetId = startWidgetId;
        s_unique_id = 0;
    }

    void InspectorUtil::DrawIndent()
    {
    }

    void InspectorUtil::DrawTitle(const std::string& p_name)
    {
        ImGui::Columns(1);
        ImGui::Columns(3,GetUniqueName(),false);
        // draw indent
        ImGui::LabelText(GetUniqueName()," ");
        ImGui::SetColumnWidth(0,(s_globalIndent-1)*s_indentSpaceFactor +0);
        ImGui::NextColumn();
        // draw title
        ImGui::TextColored(Converter::ToImVec4(GUIUtil::TitleColor), p_name.c_str());
        //ImGui::SetColumnWidth(0, -1);
        ImGui::NextColumn();
        // draw content
        // ...
    }

    const char* InspectorUtil::GetUniqueName(const std::string& p_name)
    {
        s_unique_id_str = p_name + s_startWidgetId + "1000" + std::to_string(s_unique_id++);
        return s_unique_id_str.c_str();
    }

    void InspectorUtil::RegisterTypeDrawFunction(std::string typeName, TypeDrawFunc func)
    {
        s_type2DrawFunction.emplace(typeName, func);
    }
#define __REGISTER_FUNCTION(type,funcName)\
    RegisterTypeDrawFunction(#type, [](auto* instance, auto& field){\
      ##funcName##(field.GetFieldName(), *TypeUtil::GetField<##type##>(instance, field));\
  })
    
    void InspectorUtil::CheckRegisterTypeDrawFunctions()
    {
        if (!s_type2DrawFunction.empty()) return;
        // TODO tanjp support other basic types
        __REGISTER_FUNCTION(float,DrawScalar);
        __REGISTER_FUNCTION(double,DrawScalar);
        __REGISTER_FUNCTION(int32_t,DrawScalar);
        __REGISTER_FUNCTION(int64_t,DrawScalar);
        __REGISTER_FUNCTION(uint32_t,DrawScalar);
        __REGISTER_FUNCTION(uint64_t,DrawScalar);
        __REGISTER_FUNCTION(char,DrawScalar);
        __REGISTER_FUNCTION(std::string,DrawString);
        __REGISTER_FUNCTION(bool,DrawBoolean);
        
        
    }
    
    
    void InspectorUtil::DrawUnknownInstance(std::string typeName,void* instance)
    {
        s_globalIndent++;
        auto type  =TypeUtil::GetType(typeName);
        if(type == nullptr) return;
        for (auto& field : type->GetFields())
        {
            auto fieldTypeName = field.GetFieldTypeName();
            if(s_type2DrawFunction.count(fieldTypeName) !=0 ){
                s_type2DrawFunction.at(fieldTypeName)(instance, field);
            }else
            {
                //continue;;
                if(field.HasTypeInfo())
                {
                    auto filedValue = field.Get(instance);
                    if(filedValue != nullptr)
                    {
                        if (CollapsingHeader(fieldTypeName))
                        {
                            DrawUnknownInstance(fieldTypeName,filedValue);
                        }
                    }else
                    {
                        DrawLabel(field.GetFieldName(), fieldTypeName+ ": nullptr");
                    }
                }else
                {
                    DrawLabel(field.GetFieldName(), " unknown type " +fieldTypeName );
                }
            }
        }
        s_globalIndent--;
    }
    void InspectorUtil::DrawDefault(Component* component)
    {
        s_globalIndent= 0;
        CheckRegisterTypeDrawFunctions();
        auto type = component->GetType();
        DrawUnknownInstance(type->GetTypeName(),component);
        ImGui::Columns(1);
        for (auto& element : type->GetMethods())
        {
            auto name = element.GetMethodName();
            if (DrawButton("   " + name + "   "))
            {
                element.Invoke(component);
            }
        }
        ImGui::Columns(1);
       
    }

    bool InspectorUtil::DrawButton(const std::string& p_name, FVector2 p_size, bool p_disabled)
    {
        bool clicked = ImGui::Button(GetUniqueName(p_name), Converter::ToImVec2(p_size));
        return clicked;
    }

    bool InspectorUtil::CollapsingHeader(const std::string& p_name)
    {
        ImGui::Columns(1);
        ImGui::Columns(2,GetUniqueName(),false);
        // draw indent
        ImGui::LabelText(GetUniqueName()," ");
        ImGui::SetColumnWidth(0,(s_globalIndent)*s_indentSpaceFactor +0);
        ImGui::NextColumn();
        return ImGui::CollapsingHeader(GetUniqueName(p_name),  nullptr);                      
    }

    bool InspectorUtil::DrawBoolean(const std::string& p_name, bool& p_data)
    {
        DrawTitle(p_name);
        auto oldVal = p_data;
        ImGui::Checkbox(GetUniqueName(), &p_data);
        return oldVal!= p_data;
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

    bool InspectorUtil::DrawString(const std::string& p_name, std::string& content)
    {
        DrawTitle(p_name);
        std::string previousContent = content;
        content.resize(256, '\0');
        bool enterPressed = ImGui::InputText(GetUniqueName(), &content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue);
        content = content.c_str();
        return previousContent != content;
    }

    bool InspectorUtil::DrawLabel(const std::string& p_name,const std::string& p_data)
    {
        DrawTitle(p_name);
        ImGui::TextColored(Converter::ToImVec4(GUIUtil::TitleColor), p_data.c_str());
        return true;
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
