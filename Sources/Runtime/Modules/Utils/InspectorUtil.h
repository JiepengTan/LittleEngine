#pragma once
#pragma once

#include "Core/Maths/FVector2.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FVector4.h"
#include "Core/Maths/FQuaternion.h"

#include "Modules/UI/Internal/WidgetContainer.h"
#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Widgets/Drags/DragSingleScalar.h"
#include "Modules/UI/Widgets/Drags/DragMultipleScalars.h"
#include "Modules/UI/Widgets/InputFields/InputText.h"
#include "Modules/UI/Widgets/Visual/Image.h"
#include "Core/Base/Color.h"
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
    	#define  _MIN_FLOAT  -9999999.f
    	#define  _MAX_FLOAT  +9999999.f
    	static uint64_t s_unique_id;
    	static std::string s_unique_id_str;
    	static std::string s_startWidgetId;
    	static void CreateTitle(const std::string& p_name);
    	static const char* GetUniqueName(const std::string& p_name);
    	
    public:
    	static void OnComponentStart(std::string& startWidgetId);
    	static void DrawDefault(Component* component);
    	static bool DrawButton(const std::string& p_nameconst, FVector2 p_size = FVector2(0.f, 0.f), bool p_disabled = false);

    	/*
		template <typename T>
		static bool DrawScalar(const std::string& p_name, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static bool DrawBoolean(const std::string& p_name, bool& p_data);
		static bool DrawVec2(const std::string& p_name, FVector2& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static bool DrawVec3(const std::string& p_name, FVector3& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static bool DrawVec4(const std::string& p_name, FVector4& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static bool DrawQuat(const std::string& p_name, FQuaternion& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static bool DrawString(const std::string& p_name, std::string& p_data);
		static bool DrawColor(const std::string& p_name, Color& p_color, bool p_hasAlpha = false);
		static bool DrawMesh(const std::string& p_name, Model*& p_data);
		static bool DrawTexture(const std::string& p_name, Texture*& p_data);
		static bool DrawTexture(const std::string & p_name, Texture *& p_data, std::string& guid);
		static bool DrawTexture(const std::string & p_name, TextureResPtr& p_data);
		static bool DrawShader(const std::string& p_name, Shader*& p_data);
		static bool DrawMaterial(const std::string& p_name, Material*& p_data);
		static bool DrawSound(const std::string& p_name, Sound*& p_data);
		static bool DrawAsset(const std::string& p_name, std::string& p_data);
    	*/
		
    };
}