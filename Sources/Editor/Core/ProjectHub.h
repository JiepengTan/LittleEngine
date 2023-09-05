/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Core/Renderer.h"
#include "Platform/Windowing/Context/Device.h"
#include "Platform/Windowing/Window.h"
#include "Modules/Rendering/Context/Driver.h"
#include "Modules/UI/Core/UIManager.h"
#include "Modules/UI/Panels/PanelWindow.h"

namespace LittleEditor::Core
{
	/**
	* A simple panel that allow the user to select the project to launch
	*/
	class ProjectHub
	{
	public:
		/**
		* Constructor
		*/
		ProjectHub();

		/**
		* Run the project hub logic
		*/
		std::tuple<bool, std::string, std::string> Run();

		/**
		* Setup the project hub specific context (minimalist context)
		*/
		void SetupContext();

		/**
		* Register the project (identified from the given path) into the project hub
		* @param p_path
		*/
		void RegisterProject(const std::string& p_path);

	private:
		std::unique_ptr<LittleEngine::Windowing::Context::Device>		m_device;
		std::unique_ptr<LittleEngine::Windowing::Window>				m_window;
		std::unique_ptr<LittleEngine::Rendering::Context::Driver>		m_driver;
		std::unique_ptr<LittleEngine::Rendering::Core::Renderer>		m_renderer;
		std::unique_ptr<LittleEngine::UI::Core::UIManager>				m_uiManager;

		LittleEngine::UI::Modules::Canvas m_canvas;
		std::unique_ptr<LittleEngine::UI::Panels::PanelWindow>			m_mainPanel;

		std::string m_projectPath = "";
		std::string m_projectName = "";
		bool m_readyToGo = false;
	};
}
