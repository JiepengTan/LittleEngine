/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/Internal/TransformNotifier.h"
#include "Core/Maths/FQuaternion.h"
#include "Core/Maths/FMatrix4.h"
#include "Core/Maths/FVector3.h"

namespace LittleEngine
{
	/**
	* Mathematic representation of a 3D transformation with float precision
	*/
	REFLECTION_TYPE(FTransform)
	STRUCT(FTransform, WhiteListFields)
	{
		REFLECTION_BODY(FTransform)
	public:
		/**
		* Create a transform without setting a parent
		* @param p_localPosition
		* @param p_localRotation
		* @param p_localScale
		*/
		FTransform(FVector3 p_localPosition = FVector3(0.0f, 0.0f, 0.0f), FQuaternion p_localRotation = FQuaternion::Identity, FVector3 p_localScale = FVector3(1.0f, 1.0f, 1.0f));

		/**
		* Destructor of the transform->
		* Will notify children on destruction
		*/
		~FTransform();

		/**
		* Simple callback that will treat parent notifications
		* @param p_notification
		*/
		void NotificationHandler(Internal::TransformNotifier::ENotification p_notification);

		/**
		* Defines a parent to the transform
		* @param p_parent
		*/
		void SetParent(FTransform& p_parent);

		/**
		* Set the parent to nullptr and recalculate world matrix
		* Returns true on success
		*/
		bool RemoveParent();

		/**
		* Check if the transform has a parent
		*/
		bool HasParent() const;
		
		void SetLocalMatrix(FMatrix4 p_localMatrix);
		void SetWorldMatrix(FMatrix4 p_worldMatrix);
		
		/**
		* Initialize transform with raw data from world info
		* @param p_position
		* @param p_rotation
		* @param p_scale
		*/
		void GenerateMatricesWorld(FVector3 p_position, FQuaternion p_rotation, FVector3 p_scale);

		/**
		* Initialize transform with raw data from local info
		* @param p_position
		* @param p_rotation
		* @param p_scale
		*/
		void GenerateMatricesLocal(FVector3 p_position, FQuaternion p_rotation, FVector3 p_scale);

		/**
		* Re-update world matrix to use parent transformations
		*/
		void UpdateWorldMatrix();

		/**
		* Re-update local matrix to use parent transformations
		*/
		void UpdateLocalMatrix();

		/**
		* Set the position of the transform in the local space
		* @param p_newPosition
		*/
		void SetLocalPosition(FVector3 p_newPosition);

		/**
		* Set the rotation of the transform in the local space
		* @param p_newRotation
		*/
		void SetLocalRotation(FQuaternion p_newRotation);

		/**
		* Set the scale of the transform in the local space
		* @param p_newScale
		*/
		void SetLocalScale(FVector3 p_newScale);

		/**
		* Set the position of the transform in world space
		* @param p_newPosition
		*/
		void SetWorldPosition(FVector3 p_newPosition);

		/**
		* Set the rotation of the transform in world space
		* @param p_newRotation
		*/
		void SetWorldRotation(FQuaternion p_newRotation);

		/**
		* Set the scale of the transform in world space
		* @param p_newScale
		*/
		void SetWorldScale(FVector3 p_newScale);

		/**
		* Translate in the local space
		* @param p_translation
		*/
		void TranslateLocal(const FVector3& p_translation);

		/**
		* Rotate in the local space
		* @param p_rotation
		*/
		void RotateLocal(const FQuaternion& p_rotation);

		/**
		* Scale in the local space
		* @param p_scale
		*/
		void ScaleLocal(const FVector3& p_scale);

		/**
		* Return the position in local space
		*/
		const FVector3& GetLocalPosition() const;

		/**
		* Return the rotation in local space
		*/
		const FQuaternion& GetLocalRotation() const;

		/**
		* Return the scale in local space
		*/
		const FVector3& GetLocalScale() const;

		/**
		* Return the position in world space
		*/
		const FVector3& GetWorldPosition() const;

		/**
		* Return the rotation in world space
		*/
		const FQuaternion& GetWorldRotation() const;

		/**
		* Return the scale in world space
		*/
		const FVector3& GetWorldScale() const;

		/**
		* Return the local matrix
		*/
		const FMatrix4& GetLocalMatrix() const;

		/**
		* Return the world matrix
		*/
		const FMatrix4& GetWorldMatrix() const;

		/**
		* Return the transform world forward
		*/
		FVector3 GetWorldForward() const;

		/**
		* Return the transform world up
		*/
		FVector3 GetWorldUp() const;

		/**
		* Return the transform world right
		*/
		FVector3 GetWorldRight() const;

		/**
		* Return the transform local forward
		*/
		FVector3 GetLocalForward() const;

		/**
		* Return the transform local up
		*/
		FVector3 GetLocalUp() const;

		/**
		* Return the transform local right
		*/
		FVector3 GetLocalRight() const;

	public:
		Internal::TransformNotifier Notifier;
		Internal::TransformNotifier::NotificationHandlerID m_notificationHandlerID;

	private:
		void PreDecomposeWorldMatrix();
		void PreDecomposeLocalMatrix();

		/* Pre-decomposed data to prevent multiple decomposition */
		META(Enable)
		FVector3 m_localPosition;
		META(Enable)
		FQuaternion m_localRotation;
		META(Enable)
		FVector3 m_localScale;
		FVector3 m_worldPosition;
		FQuaternion m_worldRotation;
		FVector3 m_worldScale;

		FMatrix4 m_localMatrix;
		FMatrix4 m_worldMatrix;

		FTransform*	m_parent;
	};
}
