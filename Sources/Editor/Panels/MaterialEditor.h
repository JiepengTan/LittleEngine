/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/Rendering/Resources/Shader.h"

namespace LittleEngine::Resources { class Material; }

namespace LittleEditor::Panels
{
	class MaterialEditor : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		MaterialEditor
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Refresh the material editor
		*/
		void Refresh();

		/**
		* Defines the target material of the material editor
		* @param p_newTarget
		*/
		void SetTarget(LittleEngine::Resources::Material& p_newTarget);

		/**
		* Returns the target of the material editor
		*/
		LittleEngine::Resources::Material* GetTarget() const;

		/**
		* Remove the target of the material editor (Clear the material editor)
		*/
		void RemoveTarget();

		/**
		* Launch the preview of the currently targeted material
		*/
		void Preview();

		/**
		* Reset material
		*/
		void Reset();
		
	private:
		void OnMaterialDropped();
		void OnShaderDropped();

		void CreateHeaderButtons();
		void CreateMaterialSelector();
		void CreateShaderSelector();
		void CreateMaterialSettings();
		void CreateShaderSettings();

		void GenerateShaderSettingsContent();
		void GenerateMaterialSettingsContent();

	private:
		LittleEngine::Resources::Material* m_target		= nullptr;
		LittleEngine::Rendering::Resources::Shader* m_shader	= nullptr;

		LittleEngine::UI::Widgets::Texts::Text* m_targetMaterialText	= nullptr;
		LittleEngine::UI::Widgets::Texts::Text* m_shaderText			= nullptr;

		LittleEngine::Eventing::Event<> m_materialDroppedEvent;
		LittleEngine::Eventing::Event<> m_shaderDroppedEvent;

		LittleEngine::UI::Widgets::Layout::Group* m_settings			= nullptr;
		LittleEngine::UI::Widgets::Layout::Group* m_materialSettings	= nullptr;
		LittleEngine::UI::Widgets::Layout::Group* m_shaderSettings		= nullptr;

		LittleEngine::UI::Widgets::Layout::Columns<2>* m_shaderSettingsColumns = nullptr;
		LittleEngine::UI::Widgets::Layout::Columns<2>* m_materialSettingsColumns = nullptr;
	};
}
