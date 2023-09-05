/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Core/EditorActions.h"

#include "Modules/UI/imgui/imgui.h"

#include "../Editor/Core/CameraController.h"

#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Components/CAmbientBoxLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"

LittleEditor::Core::CameraController::CameraController
(
	LittleEditor::Panels::AView& p_view,
	LittleEngine::Rendering::LowRenderer::Camera& p_camera,
	LittleEngine::FVector3& p_position,
	LittleEngine::FQuaternion& p_rotation,
	bool p_enableFocusInputs
) :
	m_inputManager(*EDITOR_CONTEXT(inputManager)),
	m_window(*EDITOR_CONTEXT(window)),
	m_view(p_view),
	m_camera(p_camera),
	m_cameraPosition(p_position),
	m_cameraRotation(p_rotation),
	m_enableFocusInputs(p_enableFocusInputs)
{
	m_camera.SetFov(60.0f);
}

float GetActorFocusDist(LittleEngine::Actor& p_actor)
{
	float distance = 4.0f;

	if (p_actor.IsActive())
	{
		if (auto pb = p_actor.GetComponent<LittleEngine::CPhysicalBox>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					pb->GetSize().x * p_actor.transform.GetWorldScale().x,
					pb->GetSize().y * p_actor.transform.GetWorldScale().y
				),
				pb->GetSize().z * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto ps = p_actor.GetComponent<LittleEngine::CPhysicalSphere>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					ps->GetRadius() * p_actor.transform.GetWorldScale().x,
					ps->GetRadius() * p_actor.transform.GetWorldScale().y
				),
				ps->GetRadius() * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto pc = p_actor.GetComponent<LittleEngine::CPhysicalCapsule>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					pc->GetRadius() * p_actor.transform.GetWorldScale().x,
					pc->GetHeight() * p_actor.transform.GetWorldScale().y
				),
				pc->GetRadius() * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto modelRenderer = p_actor.GetComponent<LittleEngine::CModelRenderer>())
		{
			const bool hasCustomBoundingSphere = modelRenderer->GetFrustumBehaviour() == LittleEngine::CModelRenderer::EFrustumBehaviour::CULL_CUSTOM;
			const bool hasModel = modelRenderer->GetModel();
			const auto boundingSphere = hasCustomBoundingSphere ? &modelRenderer->GetCustomBoundingSphere() : hasModel ? &modelRenderer->GetModel()->GetBoundingSphere() : nullptr;
			const auto& actorPosition = p_actor.transform.GetWorldPosition();
			const auto& actorScale = p_actor.transform.GetWorldScale();
			const auto scaleFactor = std::max(std::max(actorScale.x, actorScale.y), actorScale.z);

			distance = std::max(distance, boundingSphere ? (boundingSphere->radius + LittleEngine::FVector3::Length(boundingSphere->position)) * scaleFactor * 2.0f : 10.0f);
		}

		for (auto child : p_actor.GetChildren())
			distance = std::max(distance, GetActorFocusDist(*child));
	}

	return distance;
}

void LittleEditor::Core::CameraController::HandleInputs(float p_deltaTime)
{
	if (m_view.IsHovered())
	{
		UpdateMouseState();

		ImGui::GetIO().DisableMouseUpdate = m_rightMousePressed || m_middleMousePressed;

		if (!ImGui::IsAnyItemActive() && m_enableFocusInputs)
		{
			if (EDITOR_EXEC(IsAnyActorSelected()))
			{
				auto targetPos = EDITOR_EXEC(GetSelectedActor()).transform.GetWorldPosition();

				float dist = GetActorFocusDist(EDITOR_EXEC(GetSelectedActor()));

				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_F))
				{
					MoveToTarget(EDITOR_EXEC(GetSelectedActor()));
				}

				auto focusObjectFromAngle = [this, &targetPos, &dist]( const LittleEngine::FVector3& offset)
				{
					auto camPos = targetPos + offset * dist;
					auto direction = LittleEngine::FVector3::Normalize(targetPos - camPos);
					m_cameraRotation = LittleEngine::FQuaternion::LookAt(direction, abs(direction.y) == 1.0f ? LittleEngine::FVector3::Right : LittleEngine::FVector3::Up);
					m_cameraDestinations.push({ camPos, m_cameraRotation });
				};

				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_UP))			focusObjectFromAngle(LittleEngine::FVector3::Up);
				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_DOWN))		focusObjectFromAngle(-LittleEngine::FVector3::Up);
				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_RIGHT))		focusObjectFromAngle(LittleEngine::FVector3::Right);
				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_LEFT))		focusObjectFromAngle(-LittleEngine::FVector3::Right);
				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_PAGE_UP))	focusObjectFromAngle(LittleEngine::FVector3::Forward);
				if (m_inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_PAGE_DOWN))	focusObjectFromAngle(-LittleEngine::FVector3::Forward);
			}
		}
	}

	if (!m_cameraDestinations.empty())
	{
		m_currentMovementSpeed = 0.0f;

		while (m_cameraDestinations.size() != 1)
			m_cameraDestinations.pop();

		auto& [destPos, destRotation] = m_cameraDestinations.front();

		float t = m_focusLerpCoefficient * p_deltaTime;

		if (LittleEngine::FVector3::Distance(m_cameraPosition, destPos) <= 0.03f)
		{
			m_cameraPosition = destPos;
			m_cameraRotation = destRotation;
			m_cameraDestinations.pop();
		}
		else
		{
			m_cameraPosition = LittleEngine::FVector3::Lerp(m_cameraPosition, destPos, t);
			m_cameraRotation = LittleEngine::FQuaternion::Lerp(m_cameraRotation, destRotation, t);
		}
	} 
	else
	{
		if (m_rightMousePressed || m_middleMousePressed || m_leftMousePressed)
		{
			auto [xPos, yPos] = m_inputManager.GetMousePosition();

			bool wasFirstMouse = m_firstMouse;

			if (m_firstMouse)
			{
				m_lastMousePosX = xPos;
				m_lastMousePosY = yPos;
				m_firstMouse = false;
			}

			LittleEngine::FVector2 mouseOffset
			{
				static_cast<float>(xPos - m_lastMousePosX),
				static_cast<float>(m_lastMousePosY - yPos)
			};

			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;

			if (m_rightMousePressed)
			{
				HandleCameraFPSMouse(mouseOffset, wasFirstMouse);
			}
			else
			{
				if (m_middleMousePressed)
				{
					if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_LEFT_ALT) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
					{
						if (EDITOR_EXEC(IsAnyActorSelected()))
						{
							HandleCameraOrbit(mouseOffset, wasFirstMouse);
						}
					}
					else
					{
						HandleCameraPanning(mouseOffset, wasFirstMouse);
					}
				}
			}
		}

		if (m_view.IsHovered())
		{
			HandleCameraZoom();
		}

		HandleCameraFPSKeyboard(p_deltaTime);
	}
}

void LittleEditor::Core::CameraController::MoveToTarget(LittleEngine::Actor& p_target)
{
	m_cameraDestinations.push({ p_target.transform.GetWorldPosition() - m_cameraRotation * LittleEngine::FVector3::Forward * GetActorFocusDist(p_target), m_cameraRotation });
}

void LittleEditor::Core::CameraController::SetSpeed(float p_speed)
{
	m_cameraMoveSpeed = p_speed;
}

float LittleEditor::Core::CameraController::GetSpeed() const
{
	return m_cameraMoveSpeed;
}

void LittleEditor::Core::CameraController::SetPosition(const LittleEngine::FVector3 & p_position)
{
	m_cameraPosition = p_position;
}

void LittleEditor::Core::CameraController::SetRotation(const LittleEngine::FQuaternion & p_rotation)
{
	m_cameraRotation = p_rotation;
}

const LittleEngine::FVector3& LittleEditor::Core::CameraController::GetPosition() const
{
	return m_cameraPosition;
}

const LittleEngine::FQuaternion& LittleEditor::Core::CameraController::GetRotation() const
{
	return m_cameraRotation;
}

bool LittleEditor::Core::CameraController::IsRightMousePressed() const
{
	return m_rightMousePressed;
}

void LittleEditor::Core::CameraController::HandleCameraPanning(const LittleEngine::FVector2& p_mouseOffset, bool p_firstMouset)
{
	m_window.SetCursorShape(LittleEngine::Windowing::Cursor::ECursorShape::HAND);

	auto mouseOffset = p_mouseOffset * m_cameraDragSpeed;

	m_cameraPosition += m_cameraRotation * LittleEngine::FVector3::Right * mouseOffset.x;
	m_cameraPosition -= m_cameraRotation * LittleEngine::FVector3::Up * mouseOffset.y;
}

LittleEngine::FVector3 RemoveRoll(const LittleEngine::FVector3& p_ypr)
{
	LittleEngine::FVector3 result = p_ypr;

	if (result.z >= 179.0f || result.z <= -179.0f)
	{
		result.x += result.z;
		result.y = 180.0f - result.y;
		result.z = 0.0f;
	}

	if (result.x > 180.0f) result.x -= 360.0f;
	if (result.x < -180.0f) result.x += 360.0f;

	return result;
}

void LittleEditor::Core::CameraController::HandleCameraOrbit(const LittleEngine::FVector2& p_mouseOffset, bool p_firstMouse)
{
	auto mouseOffset = p_mouseOffset * m_cameraOrbitSpeed;

	if (p_firstMouse)
	{
		m_ypr = LittleEngine::FQuaternion::EulerAngles(m_cameraRotation);
		m_ypr = RemoveRoll(m_ypr);
		m_orbitTarget = &EDITOR_EXEC(GetSelectedActor()).transform.GetFTransform();
		m_orbitStartOffset = -LittleEngine::FVector3::Forward * LittleEngine::FVector3::Distance(m_orbitTarget->GetWorldPosition(), m_cameraPosition);
	}

	m_ypr.y += -mouseOffset.x;
	m_ypr.x += -mouseOffset.y;
	m_ypr.x = std::max(std::min(m_ypr.x, 90.0f), -90.0f);

	auto& target = EDITOR_EXEC(GetSelectedActor()).transform.GetFTransform();
	LittleEngine::FTransform pivotTransform(target.GetWorldPosition());
	LittleEngine::FTransform cameraTransform(m_orbitStartOffset);
	cameraTransform.SetParent(pivotTransform);
	pivotTransform.RotateLocal(LittleEngine::FQuaternion(m_ypr));
	m_cameraPosition = cameraTransform.GetWorldPosition();
	m_cameraRotation = cameraTransform.GetWorldRotation();
}

void LittleEditor::Core::CameraController::HandleCameraZoom()
{
	m_cameraPosition += m_cameraRotation * LittleEngine::FVector3::Forward * ImGui::GetIO().MouseWheel;
}

void LittleEditor::Core::CameraController::HandleCameraFPSMouse(const LittleEngine::FVector2& p_mouseOffset, bool p_firstMouse)
{
	auto mouseOffset = p_mouseOffset * m_mouseSensitivity;

	if (p_firstMouse)
	{
		m_ypr = LittleEngine::FQuaternion::EulerAngles(m_cameraRotation);
		m_ypr = RemoveRoll(m_ypr);
	}

	m_ypr.y -= mouseOffset.x;
	m_ypr.x += -mouseOffset.y;
	m_ypr.x = std::max(std::min(m_ypr.x, 90.0f), -90.0f);

	m_cameraRotation = LittleEngine::FQuaternion(m_ypr);
}

void LittleEditor::Core::CameraController::HandleCameraFPSKeyboard(float p_deltaTime)
{
	m_targetSpeed = LittleEngine::FVector3(0.f, 0.f, 0.f);

	if (m_rightMousePressed)
	{
		bool run = m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_LEFT_SHIFT) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN;
		float velocity = m_cameraMoveSpeed * p_deltaTime * (run ? 2.0f : 1.0f);

		if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_W) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_cameraRotation * LittleEngine::FVector3::Forward * velocity;
		if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_S) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_cameraRotation * LittleEngine::FVector3::Forward * -velocity;
		if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_A) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_cameraRotation * LittleEngine::FVector3::Right * velocity;
		if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_D) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_cameraRotation * LittleEngine::FVector3::Right * -velocity;
		if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_E) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += {0.0f, velocity, 0.0f};
		if (m_inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_Q) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += {0.0f, -velocity, 0.0f};
	}

	m_currentMovementSpeed = LittleEngine::FVector3::Lerp(m_currentMovementSpeed, m_targetSpeed, 10.0f * p_deltaTime);
	m_cameraPosition += m_currentMovementSpeed;
}

void LittleEditor::Core::CameraController::UpdateMouseState()
{
	if (m_inputManager.IsMouseButtonPressed(LittleEngine::Windowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
		m_leftMousePressed = true;

	if (m_inputManager.IsMouseButtonReleased(LittleEngine::Windowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_leftMousePressed = false;
		m_firstMouse = true;
	}

	if (m_inputManager.IsMouseButtonPressed(LittleEngine::Windowing::Inputs::EMouseButton::MOUSE_BUTTON_MIDDLE))
		m_middleMousePressed = true;

	if (m_inputManager.IsMouseButtonReleased(LittleEngine::Windowing::Inputs::EMouseButton::MOUSE_BUTTON_MIDDLE))
	{
		m_middleMousePressed = false;
		m_firstMouse = true;
	}

	if (m_inputManager.IsMouseButtonPressed(LittleEngine::Windowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = true;
		m_window.SetCursorMode(LittleEngine::Windowing::Cursor::ECursorMode::DISABLED);
	}

	if (m_inputManager.IsMouseButtonReleased(LittleEngine::Windowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = false;
		m_firstMouse = true;
		m_window.SetCursorMode(LittleEngine::Windowing::Cursor::ECursorMode::NORMAL);
	}
}
