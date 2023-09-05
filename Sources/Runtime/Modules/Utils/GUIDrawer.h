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
#include "Modules/UI/Types/Color.h"



namespace LittleEngine::Resources
{
	class Material;
}

namespace LittleEngine::Audio::Resources
{
	class Sound;
}

namespace LittleEngine::Rendering::Resources
{
	class Model;
	class Shader;
	class Texture;
}

namespace LittleEngine::Helpers
{
	/**
	* Provide some helpers to draw UI elements
	*/
	class GUIDrawer
	{
	public:
		static const LittleEngine::UI::Types::Color TitleColor;
		static const LittleEngine::UI::Types::Color ClearButtonColor;

		static const float _MIN_FLOAT;
		static const float _MAX_FLOAT;

		/**
		* Defines the texture to use when there is no texture in a texture resource field
		* @param p_emptyTexture
		*/
		static void ProvideEmptyTexture(LittleEngine::Rendering::Resources::Texture& p_emptyTexture);

		/**
		* Draw a title with the title color
		* @param p_root
		* @param p_name
		*/
		static void CreateTitle(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name);
	
		template <typename T>
		static void DrawScalar(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, T& p_data, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawBoolean(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, bool& p_data);
		static void DrawVec2(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::FVector2& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec3(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::FVector3& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec4(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::FVector4& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawQuat(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::FQuaternion& p_data, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawString(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::string& p_data);
		static void DrawColor(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::UI::Types::Color& p_color, bool p_hasAlpha = false);
		static LittleEngine::UI::Widgets::Texts::Text& DrawMesh(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Rendering::Resources::Model*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Visual::Image& DrawTexture(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Rendering::Resources::Texture*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawShader(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Rendering::Resources::Shader*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawMaterial(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Resources::Material*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawSound(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Audio::Resources::Sound*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);
		static LittleEngine::UI::Widgets::Texts::Text& DrawAsset(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::string& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier = nullptr);

		template <typename T>
		static void DrawScalar(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<T(void)> p_gatherer, std::function<void(T)> p_provider, float p_step = 1.f, T p_min = std::numeric_limits<T>::min(), T p_max = std::numeric_limits<T>::max());
		static void DrawBoolean(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<bool(void)> p_gatherer, std::function<void(bool)> p_provider);
		static void DrawVec2(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<LittleEngine::FVector2(void)> p_gatherer, std::function<void(LittleEngine::FVector2)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec3(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<LittleEngine::FVector3(void)> p_gatherer, std::function<void(LittleEngine::FVector3)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawVec4(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<LittleEngine::FVector4(void)> p_gatherer, std::function<void(LittleEngine::FVector4)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawQuat(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<LittleEngine::FQuaternion(void)> p_gatherer, std::function<void(LittleEngine::FQuaternion)> p_provider, float p_step = 1.f, float p_min = _MIN_FLOAT, float p_max = _MAX_FLOAT);
		static void DrawDDString(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider, const std::string& p_identifier);
		static void DrawString(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider);
		static void DrawColor(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::function<LittleEngine::UI::Types::Color(void)> p_gatherer, std::function<void(LittleEngine::UI::Types::Color)> p_provider, bool p_hasAlpha = false);

		template <typename T>
		static ImGuiDataType_ GetDataType();

		template <typename T>
		static std::string GetFormat();

	private:
		static LittleEngine::Rendering::Resources::Texture* __EMPTY_TEXTURE;
	};
}

#include "Modules/Utils/GUIDrawer.inl"
