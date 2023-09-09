/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

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
	/**
	* Provide some helpers to draw UI elements
	*/
	class GUIUtil
	{
	public:
		static const LittleEngine::Color TitleColor;
		static const LittleEngine::Color ClearButtonColor;

		static const float _MIN_FLOAT;
		static const float _MAX_FLOAT;
		static LittleEngine::UI::Internal::WidgetContainer* m_root;
		/**
		* Defines the texture to use when there is no texture in a texture resource field
		* @param p_emptyTexture
		*/
		static void ProvideEmptyTexture(LittleEngine::Texture& p_emptyTexture);

		/**
		* Draw a title with the title color
		* @param p_root
		* @param p_name
		*/
		static void CreateTitle(const std::string& p_name);
	
		template <typename T>
		static void DrawScalar(const std::string& p_name, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawBoolean(const std::string& p_name, bool& p_data);
		static void DrawVec2(const std::string& p_name, LittleEngine::FVector2& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec3(const std::string& p_name, LittleEngine::FVector3& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec4(const std::string& p_name, LittleEngine::FVector4& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawQuat(const std::string& p_name, LittleEngine::FQuaternion& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawString(const std::string& p_name, std::string& p_data);
		static void DrawColor(const std::string& p_name, LittleEngine::Color& p_color, bool p_hasAlpha = false);
		static LittleEngine::UI::Widgets::Texts::Text& DrawMesh(const std::string& p_name, LittleEngine::Model*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Visual::Image& DrawTexture(const std::string& p_name, LittleEngine::Texture*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Visual::Image& DrawTexture(const std::string & p_name, LittleEngine::Texture *& p_data, std::string& guid,  LittleEngine::Eventing::Event<>* p_updateNotifier= nullptr);
		static LittleEngine::UI::Widgets::Visual::Image& DrawTexture(const std::string & p_name, TextureResPtr& p_data,  LittleEngine::Eventing::Event<>* p_updateNotifier= nullptr);
		
		static LittleEngine::UI::Widgets::Texts::Text& DrawShader(const std::string& p_name, LittleEngine::Shader*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawMaterial(const std::string& p_name, LittleEngine::Material*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawSound(const std::string& p_name, LittleEngine::Sound*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawAsset(const std::string& p_name, std::string& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);

		template <typename T>
		static void DrawScalar(const std::string& p_name, std::function<T(void)> p_gatherer, std::function<void(T)> p_provider, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawBoolean(const std::string& p_name, std::function<bool(void)> p_gatherer, std::function<void(bool)> p_provider);
		static void DrawVec2(const std::string& p_name, std::function<LittleEngine::FVector2(void)> p_gatherer, std::function<void(LittleEngine::FVector2)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec3(const std::string& p_name, std::function<LittleEngine::FVector3(void)> p_gatherer, std::function<void(LittleEngine::FVector3)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec4(const std::string& p_name, std::function<LittleEngine::FVector4(void)> p_gatherer, std::function<void(LittleEngine::FVector4)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawQuat(const std::string& p_name, std::function<LittleEngine::FQuaternion(void)> p_gatherer, std::function<void(LittleEngine::FQuaternion)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawDDString(const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider, const std::string& p_identifier);
		static void DrawString(const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider);
		static void DrawColor(const std::string& p_name, std::function<LittleEngine::Color(void)> p_gatherer, std::function<void(LittleEngine::Color)> p_provider, bool p_hasAlpha = false);
		static std::vector<std::pair<LittleEngine::UI::Widgets::AWidget*,
				LittleEngine::UI::Internal::EMemoryMode>>& GetWidgets();
		template <typename T, typename ... Args>
		static T& CreateWidget(Args&&... p_args)
		{
			return m_root->CreateWidget<T>(p_args...);
		}
		template <typename T>
		static ImGuiDataType_ GetDataType();

		template <typename T>
		static std::string GetFormat();

	private:
		static LittleEngine::Texture* __EMPTY_TEXTURE;
	};
}

#include "Modules/Utils/GUIUtil.inl"
