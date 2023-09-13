#include "InspectorUtil.h"

#include "Core/Reflection/MetaDefine.h"
#include "ImGui/imgui_internal.h"
#include "Modules/Framework/ECS/Component.h"
#include "Modules/UI/Internal/Converter.h"
#include "Core/Reflection/TypeUtil.h"
#include "Core/Tools/Utils/PathParser.h"
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

    void InspectorUtil::DrawTitle(const std::string& p_name,int columnCount)
    {
        ImGui::Columns(1);
        ImGui::Columns(columnCount,GetUniqueName(),false);
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
        __REGISTER_FUNCTION(LittleEngine::FVector2,DrawVec2);
        __REGISTER_FUNCTION(LittleEngine::FVector3,DrawVec3);
        __REGISTER_FUNCTION(LittleEngine::FVector4,DrawVec4);
        __REGISTER_FUNCTION(LittleEngine::FQuaternion,DrawQuat);
        __REGISTER_FUNCTION(LittleEngine::Color,DrawColor);
        __REGISTER_FUNCTION(LittleEngine::ShaderResPtr,DrawAsset);
        __REGISTER_FUNCTION(LittleEngine::TextureResPtr,DrawAsset);
        __REGISTER_FUNCTION(LittleEngine::ModelResPtr,DrawAsset);
        __REGISTER_FUNCTION(LittleEngine::AnimationResPtr,DrawAsset);
        __REGISTER_FUNCTION(LittleEngine::SoundResPtr,DrawAsset);
        __REGISTER_FUNCTION(LittleEngine::MaterialResPtr,DrawAsset);

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
        float* ptr = (float*)&p_data;
        return DrawScalarN<float,2>(p_name,ptr,p_step,p_min,p_max);
    }

    bool InspectorUtil::DrawVec3(const std::string& p_name, FVector3& p_data, float p_step, float p_min, float p_max)
    {
        float* ptr = (float*)&p_data;
        return DrawScalarN<float,3>(p_name,ptr,p_step,p_min,p_max);
    }

    bool InspectorUtil::DrawVec4(const std::string& p_name, FVector4& p_data, float p_step, float p_min, float p_max)
    {
        float* ptr = (float*)&p_data;
        return DrawScalarN<float,4>(p_name,ptr,p_step,p_min,p_max);
    }

    bool InspectorUtil::DrawQuat(const std::string& p_name, FQuaternion& p_data, float p_step, float p_min, float p_max)
    {
        float* ptr = (float*)&p_data;
        return DrawScalarN<float,4>(p_name,ptr,p_step,p_min,p_max);
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

    bool InspectorUtil::DrawText(const std::string& p_name, std::string& p_data)
    {
        DrawTitle(p_name);
        auto rawLabel = p_data;
        ImGui::Text(p_data.c_str());
        return rawLabel == p_data;
    }


    bool InspectorUtil::DrawColor(const std::string& p_name, Color& p_color, bool p_hasAlpha)
    {
        DrawTitle(p_name);
	    int flags = !p_hasAlpha ? ImGuiColorEditFlags_NoAlpha : 0;
        if(p_hasAlpha)
            return  ImGui::ColorEdit4(GetUniqueName(), &p_color.r, flags);
        else
            return  ImGui::ColorEdit3(GetUniqueName(), &p_color.r, flags);
    }

#define _DrawAsset(p_name,p_resPtr,p_shaderType)\
    auto& path = p_resPtr.GetGuidReference();\
    auto& ptrs = p_resPtr.GetPtrReference();\
    bool isDirty = DrawResPtr(p_name, path, (void*&)ptrs,Utils::PathParser::EFileType::p_shaderType);\
    if(isDirty)\
        ResourcesUtils::LoadRes(path,ptrs,true);\
    return isDirty;
    
    bool InspectorUtil::DrawAsset(const std::string& p_name, ModelResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,MODEL)
    }
    bool InspectorUtil::DrawAsset(const std::string& p_name, TextureResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,TEXTURE)
    }
    bool InspectorUtil::DrawAsset(const std::string& p_name, ShaderResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,SHADER)
    }
    bool InspectorUtil::DrawAsset(const std::string& p_name, AnimationResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,MODEL)
    }
    bool InspectorUtil::DrawAsset(const std::string& p_name, MaterialResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,MATERIAL)
    }
    bool InspectorUtil::DrawAsset(const std::string& p_name, SoundResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,SOUND)
    }

    bool InspectorUtil::DrawAsset(const std::string& p_name, std::string& p_data)
    {
        return false;
    }

    
    bool InspectorUtil::DrawResPtr(const std::string& p_name, std::string& content, void*& ptrs, LittleEngine::Utils::PathParser::EFileType type)
    {
        DrawTitle(p_name,4);
        // draw input
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGui::SetColumnWidth(2,window->ContentSize.x -180);
        //ImGui::TextColored(Converter::ToImVec4(GUIUtil::ClearButtonColor), content.c_str());
        std::string previousContent = content;
        content.resize(256, '\0');
        bool enterPressed = ImGui::InputText(GetUniqueName(), &content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue);
        content = content.c_str();
        // apply DragDrop
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags target_flags = 0;
				
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File", target_flags))
            {
                auto path = *(std::string*) payload->Data;
                if (LittleEngine::Utils::PathParser::GetFileType(path) == type)
                {
                    content = path;
                    return true;
                }
            }
            ImGui::EndDragDropTarget();
        }
        // draw clean button
        ImGui::NextColumn();
        if (ImGui::Button(GetUniqueName("Clear"),ImVec2(100,0)))
        {
            content = "";
            ptrs = nullptr;
            return true;
        }
        ImGui::NextColumn();
        return false;
    }
}
