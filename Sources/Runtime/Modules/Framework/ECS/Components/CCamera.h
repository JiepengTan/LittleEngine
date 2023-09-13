/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/LowRenderer/Camera.h"
#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represents a camera entity. Its position will determine its view matrix
	*/
	REFLECTION_COMPONENT_TYPE(CCamera)
	CLASS (CCamera : public Component, WhiteListFields,CustomEditor)
	{
		REFLECTION_COMPONENT_BODY(CCamera)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void OnAwake() override;

		/**
		* Destructor
		*/
		~CCamera() = default;

		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Sets the fov of the camera to the given value
		* @param p_value
		*/
		void SetFov(float p_value);

        /**
        * Sets the size of the camera to the given value
        * @param p_value
        */
        void SetSize(float p_value);

		/**
		* Sets the near of the camera to the given value
		* @param p_value
		*/
		void SetNear(float p_value);

		/**
		* Sets the far of the camera to the given value
		* @param p_value
		*/
		void SetFar(float p_value);

		/**
		* Sets the clear color of the camera to the given value
		* @param p_value
		*/
		void SetClearColor(const LittleEngine::FVector3& p_clearColor);

		/**
		* Defines if the camera should apply frustum geometry culling in rendering
		* @param p_enable
		*/
		void SetFrustumGeometryCulling(bool p_enable);

		/**
		* Defines if the camera should apply frustum light culling in rendering
		* @param p_enable
		*/
		void SetFrustumLightCulling(bool p_enable);

        /**
        * Defines the projection mode the camera should adopt
        * @param p_projectionMode
        */
        void SetProjectionMode(LittleEngine::Rendering::Settings::EProjectionMode p_projectionMode);

		/**
		* Returns the fov of the camera
		*/
		float GetFov() const;

        /**
        * Returns the size of the camera
        */
        float GetSize() const;

		/**
		* Returns the near of the camera
		*/
		float GetNear() const;

		/**
		* Returns the far of the camera
		*/
		float GetFar() const;

		/**
		* Returns the clear color of the camera
		*/
		const LittleEngine::FVector3& GetClearColor() const;

		/**
		* Returns true if the frustum geometry culling is enabled
		*/
		bool HasFrustumGeometryCulling() const;

		/**
		* Returns true if the frustum light culling is enabled
		*/
		bool HasFrustumLightCulling() const;

        /**
        * Returns the current projection mode
        */
        LittleEngine::Rendering::Settings::EProjectionMode GetProjectionMode() const;

		/**
		* Returns the LittleEngine::Rendering camera instance attached to this component
		*/
		LittleEngine::Rendering::LowRenderer::Camera& GetCamera();

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
		LittleEngine::Rendering::LowRenderer::Camera m_camera;
	};
}
