/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <queue>

#include "Platform/Windowing/Inputs/InputManager.h"
#include "Platform/Windowing/Window.h"
#include "Modules/Rendering/LowRenderer/Camera.h"

#include "../Editor/Panels/Hierarchy.h"
#include "../Editor/Panels/AView.h"

namespace LittleEngine::Editor::Core
{
	/**
	* A simple camera controller used to navigate into views
	*/
	class CameraController
	{
	public:
		/**
		* Constructor
		* @param p_view
		* @param p_camera
		* @param p_position
		* @param p_rotation
		* @param p_enableFocusInputs
		*/
		CameraController
		(
			LittleEngine::Editor::Panels::AView& p_view,
			LittleEngine::Rendering::LowRenderer::Camera& p_camera,
			LittleEngine::FVector3& p_position,
			LittleEngine::FQuaternion& p_rotation,
			bool p_enableFocusInputs = false
		);

		/**
		* Handle mouse and keyboard inputs
		* @parma p_deltaTime
		*/
		void HandleInputs(float p_deltaTime);

		/**
		* Asks the camera to move to the target actor
		* @param p_target
		*/
		void MoveToTarget(ActorPtr p_target);

		/**
		* Defines the speed of the camera
		* @param p_speed
		*/
		void SetSpeed(float p_speed);

		/**
		* Returns the camera speed
		*/
		float GetSpeed() const;

		/**
		* Defines the position of the camera
		* @param p_position
		*/
		void SetPosition(const LittleEngine::FVector3& p_position);

		/**
		* Defines the rotation of the camera
		* @param p_rotation
		*/
		void SetRotation(const LittleEngine::FQuaternion& p_rotation);

		/**
		* Returns the position of the camera
		*/
		const LittleEngine::FVector3& GetPosition() const;

		/**
		* Returns the position of the camera
		*/
		const LittleEngine::FQuaternion& GetRotation() const;

		/**
		* Returns true if the right mouse click is being pressed
		*/
		bool IsRightMousePressed() const;

	private:
		void HandleCameraPanning(const LittleEngine::FVector2& p_mouseOffset, bool p_firstMouse);
		void HandleCameraOrbit(const LittleEngine::FVector2& p_mouseOffset, bool p_firstMouse);
		void HandleCameraFPSMouse(const LittleEngine::FVector2& p_mouseOffset, bool p_firstMouse);

		void HandleCameraZoom();
		void HandleCameraFPSKeyboard(float p_deltaTime);
		void UpdateMouseState();

	private:
		LittleEngine::Windowing::Inputs::InputManager& m_inputManager;
		LittleEngine::Windowing::Window& m_window;
		LittleEngine::Editor::Panels::AView& m_view;
		LittleEngine::Rendering::LowRenderer::Camera& m_camera;
		LittleEngine::FVector3& m_cameraPosition;
		LittleEngine::FQuaternion& m_cameraRotation;

		std::queue<std::tuple<LittleEngine::FVector3, LittleEngine::FQuaternion>> m_cameraDestinations;

		bool m_enableFocusInputs;

		bool m_leftMousePressed = false;
		bool m_middleMousePressed = false;
		bool m_rightMousePressed = false;

		LittleEngine::FVector3 m_targetSpeed;
		LittleEngine::FVector3 m_currentMovementSpeed;

		LittleEngine::FTransform* m_orbitTarget = nullptr;
		LittleEngine::FVector3 m_orbitStartOffset;
		bool m_firstMouse = true;
		double m_lastMousePosX = 0.0;
		double m_lastMousePosY = 0.0;
		LittleEngine::FVector3 m_ypr;
		float m_mouseSensitivity = 0.12f;
		float m_cameraDragSpeed = 0.03f;
		float m_cameraOrbitSpeed = 0.5f;
		float m_cameraMoveSpeed = 15.0f;
		float m_focusDistance = 15.0f;
		float m_focusLerpCoefficient = 8.0f;
	};
}
