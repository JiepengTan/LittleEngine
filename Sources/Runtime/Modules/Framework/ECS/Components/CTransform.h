/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/ECS/Component.h"

#include "Core/Maths/FTransform.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FQuaternion.h"

#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represents the 3D transformations applied to an actor
	*/
	REFLECTION_COMPONENT_TYPE(CTransform)
	CLASS (CTransform : public Component, WhiteListFields)
	{
		REFLECTION_COMPONENT_BODY(CTransform)
	public:
		
		void DoInit(ActorPtr p_owner) override;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines a parent to the transform
		* @param p_parent
		*/
		void SetParent(SharedPtr<CTransform> p_parent);

		/**
		* Set the parent to nullptr and recalculate world matrix
		* Returns true on success
		*/
		bool RemoveParent();

		/**
		* Check if the transform has a parent
		*/
		bool HasParent() const;
		void SetWorldMatrix(LittleEngine::FMatrix4 p_worldMatrix);
		void SetLocalMatrix(LittleEngine::FMatrix4 p_localMatrix);
		
		/**
		* Set the position of the transform in the local space
		* @param p_newPosition
		*/
		void SetLocalPosition(struct LittleEngine::FVector3 p_newPosition);

		/**
		* Set the rotation of the transform in the local space
		* @param p_newRotation
		*/
		void SetLocalRotation(LittleEngine::FQuaternion p_newRotation);

		/**
		* Set the scale of the transform in the local space
		* @param p_newScale
		*/
		void SetLocalScale(struct LittleEngine::FVector3 p_newScale);

	
		/**
		* Set the position of the transform in world space
		* @param p_newPosition
		*/
		void SetWorldPosition(struct LittleEngine::FVector3 p_newPosition);

		/**
		* Set the rotation of the transform in world space
		* @param p_newRotation
		*/
		void SetWorldRotation(LittleEngine::FQuaternion p_newRotation);

		/**
		* Set the scale of the transform in world space
		* @param p_newScale
		*/
		void SetWorldScale(struct LittleEngine::FVector3 p_newScale);

		/**
		* Translate in the local space
		* @param p_translation
		*/
		void TranslateLocal(const struct LittleEngine::FVector3& p_translation);

		/**
		* Rotate in the local space
		* @param p_rotation
		*/
		void RotateLocal(const LittleEngine::FQuaternion& p_rotation);

		/**
		* Scale in the local space
		* @param p_scale
		*/
		void ScaleLocal(const struct LittleEngine::FVector3& p_scale);

		/**
		* Return the position in local space
		*/
		const LittleEngine::FVector3& GetLocalPosition() const;

		/**
		* Return the rotation in local space
		*/
		const LittleEngine::FQuaternion& GetLocalRotation() const;

		/**
		* Return the scale in local space
		*/
		const LittleEngine::FVector3& GetLocalScale() const;

		/**
		* Return the position in world space
		*/
		const LittleEngine::FVector3& GetWorldPosition() const;

		/**
		* Return the rotation in world space
		*/
		const LittleEngine::FQuaternion& GetWorldRotation() const;

		/**
		* Return the scale in world space
		*/
		const LittleEngine::FVector3& GetWorldScale() const;

		/**
		* Return the local matrix
		*/
		const LittleEngine::FMatrix4& GetLocalMatrix() const;

		/**
		* Return the world matrix
		*/
		const LittleEngine::FMatrix4& GetWorldMatrix() const;

		/**
		* Return the FTransform attached to the CTransform
		*/
		LittleEngine::FTransform& GetFTransform();

		/**
		* Return the transform world forward
		*/
		LittleEngine::FVector3 GetWorldForward() const;

		/**
		* Return the transform world up
		*/
		LittleEngine::FVector3 GetWorldUp() const;

		/**
		* Return the transform world right
		*/
		LittleEngine::FVector3 GetWorldRight() const;

		/**
		* Return the transform local forward
		*/
		LittleEngine::FVector3 GetLocalForward() const;

		/**
		* Return the transform local up
		*/
		LittleEngine::FVector3 GetLocalUp() const;

		/**
		* Return the transform local right
		*/
		LittleEngine::FVector3 GetLocalRight() const;

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(ISerializer p_serializer) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(ISerializer p_serializer) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;

	private:
		META(Enable)
		LittleEngine::FTransform m_transform;
	};
}
