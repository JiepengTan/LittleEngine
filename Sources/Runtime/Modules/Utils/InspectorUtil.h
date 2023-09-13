#pragma once
#pragma once

#include "GUIUtil.h"
#include "Core/Maths/FVector2.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FVector4.h"
#include "Core/Maths/FQuaternion.h"

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Core/Base/Color.h"
#include "Core/Tools/Utils/PathParser.h"
#include "Resource/Core/ResPtr.h"

namespace LittleEngine
{
    class Model;
    class Shader;
    class Texture;
    class Sound;
    class Material;
}

namespace LittleEngine
{
	class Component;
    class InspectorUtil
    {
    	typedef std::function<void(void*,Reflection::FieldAccessor&)> TypeDrawFunc; 
    	static uint64_t s_unique_id;
    	static std::string s_unique_id_str;
    	static std::string s_startWidgetId;
    	static int s_globalIndent;
    	static int s_indentSpaceFactor;
    	static std::unordered_map<std::string,TypeDrawFunc> s_type2DrawFunction;
    	
    	static void DrawIndent();
    	static void DrawTitle(const std::string& p_name,int columnCount = 3);
    	static const char* GetUniqueName(const std::string& p_name = "");
    	static void RegisterTypeDrawFunction(std::string typeName,TypeDrawFunc func);
	    static void CheckRegisterTypeDrawFunctions();
    	static void DrawUnknownInstance(std::string typeName,void* instance);
    	static bool DrawResPtr(const std::string& p_name, std::string& content, void*& ptrs,
			LittleEngine::Utils::PathParser::EFileType type,uint32_t texId = 0);
    public:
    	static void OnComponentStart(std::string& startWidgetId);

    	
    public:
    	static void DrawDefault(Component* component);

    	static bool DrawString(const std::string& p_name, std::string& p_data);
    	static bool DrawLabel(const std::string& p_name,const std::string& p_data);
    	static bool DrawText(const std::string& p_name, std::string& p_data);
    	static bool DrawColor(const std::string& p_name, Color& p_color, bool p_hasAlpha = true);
    	
    	template <typename T, int _Size>
		static bool DrawScalarN(const std::string& p_name, T*& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		template <typename T>
		static bool DrawScalar(const std::string& p_name, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static bool DrawBoolean(const std::string& p_name, bool& p_data);
		static bool DrawVec2(const std::string& p_name, FVector2& p_data, float p_step = 1.f, float p_min = GUIUtil::_MIN_FLOAT, float p_max = GUIUtil::_MAX_FLOAT);
		static bool DrawVec3(const std::string& p_name, FVector3& p_data, float p_step = 1.f, float p_min = GUIUtil::_MIN_FLOAT, float p_max = GUIUtil::_MAX_FLOAT);
		static bool DrawVec4(const std::string& p_name, FVector4& p_data, float p_step = 1.f, float p_min = GUIUtil::_MIN_FLOAT, float p_max = GUIUtil::_MAX_FLOAT);
		static bool DrawQuat(const std::string& p_name, FQuaternion& p_data, float p_step = 1.f, float p_min = GUIUtil::_MIN_FLOAT, float p_max = GUIUtil::_MAX_FLOAT);

    	
		static bool DrawAsset(const std::string& p_name, ModelResPtr& p_data);
		static bool DrawAsset(const std::string& p_name, TextureResPtr& p_data);
	    static bool DrawAsset(const std::string& p_name, ShaderResPtr& p_data);
		static bool DrawAsset(const std::string& p_name, AnimationResPtr& p_data);
		static bool DrawAsset(const std::string& p_name, MaterialResPtr& p_data);
		static bool DrawAsset(const std::string& p_name, SoundResPtr& p_data);
		static bool DrawAsset(const std::string& p_name, std::string& p_data);
    	
    	static bool DrawButton(const std::string& p_nameconst, FVector2 p_size = FVector2(0.f, 0.f), bool p_disabled = false);
    	static bool DrawCollapsingHeader(const std::string& p_name);
    		
    };

	template <typename T, int _Size>
	bool InspectorUtil::DrawScalarN(const std::string& p_name, T*& p_data, float p_step, T p_min, T p_max)
	{
		DrawTitle(p_name);
		auto m_dataType =GUIUtil:: GetDataType<T>();
		auto format = GUIUtil::GetFormat<T>();
    	
		auto min = p_min;
		auto max = p_max;
		if (max < min)
		{
			auto temp = max;
			max = min;
			min = max;
		}
		for (size_t i = 0; i < _Size; ++i)
		{
			if (p_data[i] < min)
				p_data[i] = min;
			else if (p_data[i] > max)
				p_data[i] = max;
		}
		if (ImGui::DragScalarN(GetUniqueName(), m_dataType, p_data, _Size, p_step, &min, &max, format.c_str()))
		{
			return true;
		}
		return false;
	}
    template <typename T>
    bool InspectorUtil::DrawScalar(const std::string& p_name, T& p_data, float p_step, T p_min, T p_max)
    {
    	DrawTitle(p_name);
		
    	auto m_dataType =GUIUtil:: GetDataType<T>();
    	auto format = GUIUtil::GetFormat<T>();
    	
    	auto min = p_min;
    	auto max = p_max;
    	if (max < min)
    	{
    		auto temp = max;
    		max = min;
    		min = max;
    	}

    	if (p_data < min)
    		p_data = min;
    	else if (p_data > max)
    		p_data = max;

    	if (ImGui::DragScalar(GetUniqueName(), m_dataType, &p_data, p_step, &min, &max, format.c_str()))
    	{
    		return true;
    	}
    	
    	return false;
    }
}
