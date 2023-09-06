/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <variant>

#include "Core/Tools/Filesystem/IniFile.h"
#include "Core/Tools/Eventing/Event.h"

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/Columns.h"

#include "Modules/Rendering/Resources/Model.h"
#include "Modules/Rendering/Resources/Texture.h"

namespace LittleEngine::Editor::Panels
{
	class AssetProperties : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		using EditableAssets = std::variant<LittleEngine::Rendering::Resources::Model*, LittleEngine::Rendering::Resources::Texture*>;

		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		AssetProperties
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Defines the target of the asset settings editor
		* @param p_path
		*/
		void SetTarget(const std::string& p_path);

        /**
        * Refresh the panel to show the current target settings
        */
        void Refresh();

		/**
		* Launch the preview of the target asset
		*/
		void Preview();

	private:
		void CreateHeaderButtons();
        void CreateAssetSelector();
		void CreateSettings();
		void CreateInfo();
		void CreateModelSettings();
		void CreateTextureSettings();
		void Apply();

	private:
		std::string m_resource;

        LittleEngine::Eventing::Event<> m_targetChanged;
        LittleEngine::UI::Widgets::Layout::Group* m_settings = nullptr;
        LittleEngine::UI::Widgets::Layout::Group* m_info = nullptr;
        LittleEngine::UI::Widgets::Buttons::Button* m_applyButton = nullptr;
        LittleEngine::UI::Widgets::Buttons::Button* m_revertButton = nullptr;
        LittleEngine::UI::Widgets::Buttons::Button* m_previewButton = nullptr;
        LittleEngine::UI::Widgets::Buttons::Button* m_resetButton = nullptr;
        LittleEngine::UI::Widgets::AWidget* m_headerSeparator = nullptr;
        LittleEngine::UI::Widgets::AWidget* m_headerLineBreak = nullptr;
		LittleEngine::UI::Widgets::Layout::Columns<2>* m_settingsColumns = nullptr;
		LittleEngine::UI::Widgets::Layout::Columns<2>* m_infoColumns = nullptr;
        LittleEngine::UI::Widgets::Texts::Text* m_assetSelector = nullptr;
		std::unique_ptr<LittleEngine::Filesystem::IniFile> m_metadata;
	};
}
