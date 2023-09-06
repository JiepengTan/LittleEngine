/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Core/GizmoBehaviour.h"
#include "../Editor/Core/EditorActions.h"
#include "../Editor/Settings/EditorSettings.h"

float SnapValue(float p_value, float p_step)
{
	return p_value - std::fmod(p_value, p_step);
}

bool LittleEngine::Editor::Core::GizmoBehaviour::IsSnappedBehaviourEnabled() const
{
	using namespace LittleEngine::Windowing::Inputs;

	const auto& inputManager = EDITOR_CONTEXT(inputManager);
	return inputManager->GetKeyState(EKey::KEY_LEFT_CONTROL) == EKeyState::KEY_DOWN || inputManager->GetKeyState(EKey::KEY_RIGHT_CONTROL) == EKeyState::KEY_DOWN;
}

void LittleEngine::Editor::Core::GizmoBehaviour::StartPicking(ActorPtr p_target, const LittleEngine::FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
	m_target = p_target;
	m_firstMouse = true;
	m_originalTransform = p_target->transform->GetFTransform();
	m_distanceToActor = LittleEngine::FVector3::Distance(p_cameraPosition, m_target->transform->GetWorldPosition());
	m_currentOperation = p_operation;
	m_direction = p_direction;
}

void LittleEngine::Editor::Core::GizmoBehaviour::StopPicking()
{
	m_target = nullptr;
}

LittleEngine::FVector3 LittleEngine::Editor::Core::GizmoBehaviour::GetFakeDirection() const
{
	auto result = LittleEngine::FVector3();

	switch (m_direction)
	{
	case LittleEngine::Editor::Core::GizmoBehaviour::EDirection::X:
		result = LittleEngine::FVector3::Right;
		break;
	case LittleEngine::Editor::Core::GizmoBehaviour::EDirection::Y:
		result = LittleEngine::FVector3::Up;
		break;
	case LittleEngine::Editor::Core::GizmoBehaviour::EDirection::Z:
		result = LittleEngine::FVector3::Forward;
		break;
	}

	return result;
}

LittleEngine::FVector3 LittleEngine::Editor::Core::GizmoBehaviour::GetRealDirection(bool p_relative) const
{
	auto result = LittleEngine::FVector3();

	switch (m_direction)
	{
	case LittleEngine::Editor::Core::GizmoBehaviour::EDirection::X:
		result = p_relative ? m_originalTransform.GetWorldRight() : m_originalTransform.GetLocalRight();
		break;
	case LittleEngine::Editor::Core::GizmoBehaviour::EDirection::Y:
		result = p_relative ? m_originalTransform.GetWorldUp() : m_originalTransform.GetLocalUp();
		break;
	case LittleEngine::Editor::Core::GizmoBehaviour::EDirection::Z:
		result = p_relative ? m_originalTransform.GetWorldForward() : m_originalTransform.GetLocalForward();
		break;
	}

	return result;
}

LittleEngine::FVector2 LittleEngine::Editor::Core::GizmoBehaviour::GetScreenDirection(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const
{
	auto start = m_originalTransform.GetWorldPosition();
	auto end = m_originalTransform.GetWorldPosition() + GetRealDirection(true) * 0.01f;

	auto start2D = LittleEngine::FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * LittleEngine::FVector4{ start.x, start.y, start.z, 1.0f });
		auto ndcSpacePos = LittleEngine::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((LittleEngine::FVector2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		start2D = windowSpacePos;
	}

	auto end2D = LittleEngine::FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * LittleEngine::FVector4{ end.x, end.y, end.z, 1.0f });
		auto ndcSpacePos = LittleEngine::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((LittleEngine::FVector2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		end2D = windowSpacePos;
	}

	auto result = end2D - start2D;

	result.y *= -1; // Screen coordinates are reversed, so we inverse the Y

	return LittleEngine::FVector2::Normalize(result);
}

void LittleEngine::Editor::Core::GizmoBehaviour::ApplyTranslation(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.001f * m_distanceToActor;
	auto originPosition = m_originalTransform.GetWorldPosition();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto translationCoefficient = LittleEngine::FVector2::Dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		translationCoefficient = SnapValue(translationCoefficient, LittleEngine::Editor::Settings::EditorSettings::TranslationSnapUnit);
	}

	m_target->transform->SetWorldPosition(originPosition + GetRealDirection(true) * translationCoefficient);
}

void LittleEngine::Editor::Core::GizmoBehaviour::ApplyRotation(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.2f;
	auto originRotation = m_originalTransform.GetWorldRotation();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);
	screenDirection = LittleEngine::FVector2(-screenDirection.y, screenDirection.x);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto rotationCoefficient = LittleEngine::FVector2::Dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		rotationCoefficient = SnapValue(rotationCoefficient, LittleEngine::Editor::Settings::EditorSettings::RotationSnapUnit);
	}

	auto rotationToApply = LittleEngine::FQuaternion(LittleEngine::FVector3(GetFakeDirection() * rotationCoefficient));
	m_target->transform->SetWorldRotation(originRotation * rotationToApply);
}

void LittleEngine::Editor::Core::GizmoBehaviour::ApplyScale(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.01f;
	auto originScale = m_originalTransform.GetWorldScale();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto scaleCoefficient = LittleEngine::FVector2::Dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		scaleCoefficient = SnapValue(scaleCoefficient, LittleEngine::Editor::Settings::EditorSettings::ScalingSnapUnit);
	}

	auto newScale = originScale + GetFakeDirection() * scaleCoefficient;

	/* Prevent scale from being negative*/
	newScale.x = std::max(newScale.x, 0.0001f);
	newScale.y = std::max(newScale.y, 0.0001f);
	newScale.z = std::max(newScale.z, 0.0001f);

	m_target->transform->SetWorldScale(newScale);
}

void LittleEngine::Editor::Core::GizmoBehaviour::ApplyOperation(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize)
{
	switch (m_currentOperation)
	{
	case EGizmoOperation::TRANSLATE:
		ApplyTranslation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;

	case EGizmoOperation::ROTATE:
		ApplyRotation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;

	case EGizmoOperation::SCALE:
		ApplyScale(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;
	}
}

void LittleEngine::Editor::Core::GizmoBehaviour::SetCurrentMouse(const LittleEngine::FVector2& p_mousePosition)
{
	if (m_firstMouse)
	{
		m_currentMouse = m_originMouse = p_mousePosition;
		m_firstMouse = false;
	}
	else
	{
		m_currentMouse = p_mousePosition;
	}
}

bool LittleEngine::Editor::Core::GizmoBehaviour::IsPicking() const
{
	return m_target != nullptr;
}

LittleEngine::Editor::Core::GizmoBehaviour::EDirection LittleEngine::Editor::Core::GizmoBehaviour::GetDirection() const
{
	return m_direction;
}
