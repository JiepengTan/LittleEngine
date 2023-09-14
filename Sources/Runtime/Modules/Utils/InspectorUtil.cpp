#include "InspectorUtil.h"

#include "Core/Reflection/MetaDefine.h"
#include "ImGui/imgui_internal.h"
#include "Modules/Framework/ECS/Component.h"
#include "Modules/UI/Internal/Converter.h"
#include "Core/Reflection/TypeUtil.h"
#include "Core/Tools/Utils/PathParser.h"
#include "Resource/Asset/Texture.h"
using namespace LittleEngine::UI::Internal;

namespace LittleEngine
{
    uint64_t InspectorUtil::s_unique_id = 0;
    std::string InspectorUtil::s_unique_id_str = "";
    std::string InspectorUtil::s_startWidgetId = "";
    int InspectorUtil::s_globalIndent = 0;
    int InspectorUtil::s_indentSpaceFactor = 20;
    std::unordered_map<std::string, InspectorUtil::TypeDrawFunc> InspectorUtil::s_type2DrawFunction;

    void InspectorUtil::OnComponentStart(std::string& p_startWidgetId)
    {
        s_startWidgetId = p_startWidgetId;
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

    void InspectorUtil::RegisterTypeDrawFunction(std::string p_typeName, TypeDrawFunc p_func)
    {
        s_type2DrawFunction.emplace(p_typeName, p_func);
    }
#define __REGISTER_FUNCTION(type,funcName)\
    RegisterTypeDrawFunction(#type, [](auto* instance, auto& field)->bool{\
      return  funcName##(field.GetFieldName(), *TypeUtil::GetField<##type##>(instance, field));\
    })
    
    void InspectorUtil::CheckRegisterTypeDrawFunctions()
    {
        if (!s_type2DrawFunction.empty()) return;
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


    void InspectorUtil::DrawFieldMeta(TypeInfoPtr p_type,void* p_instance, 
        Reflection::FieldAccessor& p_field, bool p_isDirty)
    {
        if(p_isDirty && p_field.HasMeta(MetaDefine::OnValueChanged))
        {
            auto funcName = p_field.GetMeta(MetaDefine::OnValueChanged);
            auto funcs = p_type->GetMethodByName(funcName);
            if(funcs != nullptr)
            {
                auto val = p_field.Get(p_instance);
                funcs->Invoke(nullptr,p_instance,val);
            }
        }
    }

    bool InspectorUtil::DrawEnum(void* p_instance, Reflection::FieldAccessor& p_field)
    {
        DrawTitle(p_field.GetFieldName());
        auto fieldTypeName = p_field.GetFieldTypeName();
        auto curValue = TypeUtil::EnumToString(fieldTypeName,p_field.Get(p_instance));
        auto enumNames = TypeUtil::GetEnumNameVector(fieldTypeName);
        // if value invalid ,set to first number
        if(curValue.empty())
        {
            curValue = enumNames[0];
            TypeUtil::EnumFromString(fieldTypeName,curValue,p_field.Get(p_instance));
        }
        int count = enumNames.size();
        if (ImGui::BeginCombo(GetUniqueName(), curValue.c_str()))
        {
            for (int i=0;i<count;i++)
            {
                bool selected =enumNames[i] == curValue;

                if (ImGui::Selectable(enumNames[i].c_str(), selected))
                {
                    if (!selected)
                    {
                        ImGui::SetItemDefaultFocus();
                        TypeUtil::EnumFromString(fieldTypeName,enumNames[i],p_field.Get(p_instance));
                        ImGui::EndCombo();
                        return true;
                    }
                }
            }
            ImGui::EndCombo();
        }
        return false;
    }

    void InspectorUtil::DrawInstance(std::string p_typeName,void* p_instance)
    {
        s_globalIndent++;
        auto type  =TypeUtil::GetType(p_typeName);
        if(type == nullptr) return;
        for (auto& field : type->GetFields())
        {
            auto fieldTypeName = field.GetFieldTypeName();
            auto fieldType  = TypeUtil::GetType(fieldTypeName);
            if(fieldType != nullptr && fieldType->IsEnum())
            {
                // draw enum
                DrawEnum(p_instance,field);
                continue;
            }
            if(s_type2DrawFunction.count(fieldTypeName) !=0 ){
                bool isDirty = s_type2DrawFunction.at(fieldTypeName)(p_instance, field);
                // handle Meta OnValueChanged 
                DrawFieldMeta(type, p_instance, field, isDirty);
            }else
            {
                //continue;;
                if(field.HasTypeInfo())
                {
                    auto filedValue = field.Get(p_instance);
                    if(filedValue != nullptr)
                    {
                        if (DrawCollapsingHeader(fieldTypeName))
                        {
                            DrawInstance(fieldTypeName,filedValue);
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

    void InspectorUtil::DrawMethodMeta(Component* p_component)
    {
        auto type = p_component->GetType();
        for (auto& element : type->GetMethods())
        {
            auto name = element.GetMethodName();
            if(element.HasMeta(MetaDefine::Button))
            {
                if (DrawButton("   " + name + "   "))
                {
                    element.Invoke(nullptr,p_component);
                }
            }
        }
    }
    void InspectorUtil::DrawDefault(Component* p_component)
    {
        s_globalIndent= 0;
        CheckRegisterTypeDrawFunctions();
        auto type = p_component->GetType();
        DrawInstance(type->GetTypeName(),p_component);
        ImGui::Columns(1);
        DrawMethodMeta(p_component);
        ImGui::Columns(1);
       
    }

    bool InspectorUtil::DrawButton(const std::string& p_name, FVector2 p_size, bool p_disabled)
    {
        bool clicked = ImGui::Button(GetUniqueName(p_name), Converter::ToImVec2(p_size));
        return clicked;
    }

    bool InspectorUtil::DrawCollapsingHeader(const std::string& p_name)
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

#define _DrawRawAsset(p_name,p_resPtr,p_shaderType,p_texId)\
    auto& path = p_resPtr.GetGuidReference();\
    auto& ptrs = p_resPtr.GetPtrReference();\
    bool isDirty = DrawResPtr(p_name, path, (void*&)ptrs,Utils::PathParser::EFileType::p_shaderType,p_texId);\
    if(isDirty)\
        ResourcesUtils::LoadRes(path,ptrs,true);\
    return isDirty;
    
#define _DrawAsset(p_name,p_resPtr,p_shaderType)\
    _DrawRawAsset(p_name,p_resPtr,p_shaderType,0)
    
    bool InspectorUtil::DrawAsset(const std::string& p_name, TextureResPtr& p_data)
    {
        auto texId = (p_data.GetPtr() == nullptr)? 0:p_data.GetPtr()->id;
        _DrawRawAsset(p_name,p_data,TEXTURE,texId)
    }
    
    bool InspectorUtil::DrawAsset(const std::string& p_name, ModelResPtr& p_data)
    {
        _DrawAsset(p_name,p_data,MODEL)
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


    
    bool InspectorUtil::DrawResPtr(const std::string& p_name, std::string& p_content, void*& p_ptr,
        LittleEngine::Utils::PathParser::EFileType p_type,uint32_t p_texId )
    {
        DrawTitle(p_name,4);
        // draw input
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGui::SetColumnWidth(2,window->ContentSize.x -180);
        //ImGui::TextColored(Converter::ToImVec4(GUIUtil::ClearButtonColor), content.c_str());
        if(p_type == Utils::PathParser::EFileType::TEXTURE)
        {
            p_texId = p_texId ==0 ?(GUIUtil::__EMPTY_TEXTURE ? GUIUtil::__EMPTY_TEXTURE->id : 0): p_texId ;
            ImGui::Image((void*)p_texId, ImVec2(75,75), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
        }else
        {
            std::string previousContent = p_content;
            p_content.resize(256, '\0');
            bool enterPressed = ImGui::InputText(GetUniqueName(), &p_content[0], 256, ImGuiInputTextFlags_EnterReturnsTrue);
            p_content = p_content.c_str();
        }
        // apply DragDrop
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags target_flags = 0;
				
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File", target_flags))
            {
                auto path = *(std::string*) payload->Data;
                if (LittleEngine::Utils::PathParser::GetFileType(path) == p_type)
                {
                    p_content = path;
                    return true;
                }
            }
            ImGui::EndDragDropTarget();
        }
        // draw clean button
        ImGui::NextColumn();
        if (ImGui::Button(GetUniqueName("Clear"),ImVec2(100,0)))
        {
            p_content = "";
            p_ptr = nullptr;
            return true;
        }
        ImGui::NextColumn();
        return false;
    }
}
