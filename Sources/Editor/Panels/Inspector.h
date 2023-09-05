/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Resources/Loaders/TextureLoader.h"

#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Layout/GroupCollapsable.h"
#include "Modules/UI/Widgets/InputFields/InputText.h"
#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Widgets/Selection/CheckBox.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Selection/ComboBox.h"

#include "../Editor/Panels/Hierarchy.h"
#include "../Editor/Panels/AssetBrowser.h"

namespace LittleEditor::Panels
{
	class Inspector : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Inspector
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Destructor
		*/
		~Inspector();

		/**
		* Focus the given actor
		* @param p_target
		*/
		void FocusActor(LittleEngine::Actor& p_target);

		/**
		* Unfocus the currently targeted actor
		*/
		void UnFocus();

		/**
		* Unfocus the currently targeted actor without removing listeners attached to this actor
		*/
		void SoftUnFocus();

		/**
		* Returns the currently selected actor
		*/
		LittleEngine::Actor* GetTargetActor() const;

		/**
		* Create the actor inspector for the given actor
		*/
		void CreateActorInspector(LittleEngine::Actor& p_target);

		/**
		* Draw the given component in inspector
		*/
		void DrawComponent(LittleEngine::Component& p_component);

		/**
		* Refresh the inspector
		*/
		void Refresh();

	private:
		LittleEngine::Actor* m_targetActor = nullptr;
		LittleEngine::UI::Widgets::Layout::Group* m_actorInfo;
		LittleEngine::UI::Widgets::Layout::Group* m_inspectorHeader;
		LittleEngine::UI::Widgets::Selection::ComboBox* m_componentSelectorWidget;
        LittleEngine::UI::Widgets::InputFields::InputText* m_scriptSelectorWidget;

		uint64_t m_componentAddedListener	= 0;
		uint64_t m_componentRemovedListener = 0;
		uint64_t m_destroyedListener		= 0;
	};
}
