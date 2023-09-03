/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Panels/PanelMenuBar.h"
#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Menu/MenuItem.h"
#include "Modules/UI/Widgets/Buttons/ButtonImage.h"

namespace OvEditor::Panels
{
	class MenuBar : public OvUI::Panels::PanelMenuBar
	{
		using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<OvUI::Panels::PanelWindow>, std::reference_wrapper<OvUI::Widgets::Menu::MenuItem>>>;

	public:
		/**
		* Constructor
		*/
		MenuBar();
		
		/**
		* Check inputs for menubar shortcuts
		* @param p_deltaTime
		*/
		void HandleShortcuts(float p_deltaTime);

		/**
		* Register a panel to the menu bar window menu
		*/
		void RegisterPanel(const std::string& p_name, OvUI::Panels::PanelWindow& p_panel);

	private:
		void CreateFileMenu();
		void CreateEditMenu();
		void CreateWindowMenu();
		void CreateActorsMenu();
		void CreateResourcesMenu();
		void CreateSettingsMenu();
		void CreateLayoutMenu();
		void CreateHelpMenu();
		void CreateToolbar();
		void UpdateToggleableItems();
		void OpenEveryWindows(bool p_state);
		OvUI::Widgets::Buttons::ButtonImage* CreateToolbarItem(std::string p_name,float& p_offset);
	private:
		PanelMap m_panels;
		const std::string PorjectSettingName = "Project Settings";
		OvUI::Widgets::Menu::MenuList* m_windowMenu = nullptr;
		OvUI::Widgets::Menu::MenuList* m_editMenu = nullptr;
		OvUI::Widgets::Buttons::ButtonImage* m_playButton;
		OvUI::Widgets::Buttons::ButtonImage* m_pauseButton;
		OvUI::Widgets::Buttons::ButtonImage* m_nextButton;
	};
}
