/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/FTransform.h"
#include "Core/Maths/FVector2.h"

#include "Modules/Framework/ECS/Actor.h"

namespace LittleEngine::Editor::Core
{
	enum class EGizmoOperation
	{
		TRANSLATE,
		ROTATE,
		SCALE
	};

	/* Handle gizmo behaviours */
	class GizmoBehaviour
	{
	public:
		enum class EDirection
		{
			X,
			Y,
			Z
		};
		
		/**
		* Returns true if the snapping behaviour is enabled
		*/
		bool IsSnappedBehaviourEnabled() const;

		/**
		* Starts the gizmo picking behaviour for the given target in the given direction
		* @param p_actor
		* @param p_cameraPosition
		* @param p_operation
		* @param p_direction
		*/
		void StartPicking(ActorPtr p_target, const LittleEngine::FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction);

		/**
		* Stops the gizmo picking behaviour
		*/
		void StopPicking();

		/**
		* Handle the current behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyOperation(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize);

		/**
		* Set the given mouse position as the current mouse position and update the previous mouse position
		* @param p_mousePosition
		*/
		void SetCurrentMouse(const LittleEngine::FVector2& p_mousePosition);

		/**
		* Returns true if the gizmo is currently picked
		*/
		bool IsPicking() const;

		/**
		* Returns the current operation direction
		*/
		EDirection GetDirection() const;

	private:
		/**
		* Returns the global direction matching with the current m_direction
		*/
		LittleEngine::FVector3 GetFakeDirection() const;

		/**
		* Returns the actual direction of the target matching with the current m_direction
		* @param p_relative (If true, the direction depends on hierarchy)
		*/
		LittleEngine::FVector3 GetRealDirection(bool p_relative = false) const;

		/**
		* Returns the 3D vector of the arrow projected to the screen
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		LittleEngine::FVector2 GetScreenDirection(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const;

		/**
		* Handle the translation behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyTranslation(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const;

		/**
		* Handle the rotation behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyRotation(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const;

		/**
		* Handle the scale behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyScale(const LittleEngine::FMatrix4& p_viewMatrix, const LittleEngine::FMatrix4& p_projectionMatrix, const LittleEngine::FVector2& p_viewSize) const;

	private:
		bool m_firstMouse = true;
		float m_distanceToActor = 0.0f;
		ActorPtr m_target = nullptr;
		EGizmoOperation m_currentOperation;
		EDirection m_direction;
		LittleEngine::FTransform m_originalTransform;
		LittleEngine::FVector2 m_originMouse;
		LittleEngine::FVector2 m_currentMouse;
		LittleEngine::FVector2 m_screenDirection;
	};
}
