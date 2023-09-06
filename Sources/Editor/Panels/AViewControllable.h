/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Editor/Panels/AView.h"
#include "../Editor/Core/CameraController.h"

namespace LittleEngine::Editor::Panels
{
	class AViewControllable : public LittleEngine::Editor::Panels::AView
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_enableFocusInputs
		*/
		AViewControllable
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings,
			bool p_enableFocusInputs = false
		);

		/**
		* Update the controllable view (Handle inputs)
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Returns the camera controller of the controllable view
		*/
		LittleEngine::Editor::Core::CameraController& GetCameraController();

	protected:
		LittleEngine::Editor::Core::CameraController m_cameraController;
	};
}
