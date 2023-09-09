/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/LowRenderer/Camera.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Resource/Asset/Material.h"
#include "Modules/Framework/ECS/Components/CAmbientBoxLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"

#include "../Editor/Core/Context.h"

namespace LittleEngine::Editor::Core { enum class EGizmoOperation; }
namespace LittleEngine::Editor::Panels { class AView; }

namespace LittleEngine::Editor::Core
{
	/**
	* Handle the rendering of the editor
	*/
	class EditorRenderer
	{
	public:
		/**
		* Create the EditorRenderer
		* @param p_context
		*/
		EditorRenderer(Context& p_context);

		/**
		* Initialize custom materials
		*/
		void InitMaterials();

		/**
		* Prepare the picking material by send it the color corresponding to the given actor
		* @param p_actor
		* @param p_material
		*/
		void PreparePickingMaterial(ActorPtr p_actor, LittleEngine::Material& p_material);

		/**
		* Calculate the model matrix for a camera attached to the given actor
		* @param p_actor
		*/
		LittleEngine::FMatrix4 CalculateCameraModelMatrix(ActorPtr p_actor);

		/**
		* Render the scene
		* @param p_cameraPosition
		* @param p_camera
		*/
		void RenderScene(const LittleEngine::FVector3& p_cameraPosition, const LittleEngine::Rendering::LowRenderer::Camera& p_camera, const LittleEngine::Rendering::Data::Frustum* p_customFrustum = nullptr);

		/**
		* Render the scene for actor picking (Unlit version of the scene with colors indicating actor IDs)
		*/
		void RenderSceneForActorPicking();

		/**
		* Render the User Interface
		*/
		void RenderUI();

		/**
		* Render every scene cameras
		*/
		void RenderCameras();

		/**
		* Render every scene lights as billboards
		*/
		void RenderLights();

		/**
		* Render a gizmo at position
		* @param p_position
		* @param p_rotation
		* @param p_operation
		* @param p_pickable (Determine the shader to use to render the gizmo)
		* @param p_highlightedAxis (-1 to highlight no axis, 0 for X, 1 for Y, 2 for Z)
		*/
		void RenderGizmo(const LittleEngine::FVector3& p_position, const LittleEngine::FQuaternion& p_rotation, LittleEngine::Editor::Core::EGizmoOperation p_operation, bool p_pickable, int p_highlightedAxis = -1);

		/**
		* Render a model to the stencil buffer
		*/
		void RenderModelToStencil(const LittleEngine::FMatrix4& p_worldMatrix, LittleEngine::Model& p_model);

		/**
		* Render a model outline using the data stored in the stencil buffer
		* @param p_worldMatrix
		* @param p_model
		* @param p_width
		*/
		void RenderModelOutline(const LittleEngine::FMatrix4& p_worldMatrix, LittleEngine::Model& p_model, float p_width);

		/**
		* Render the actor as a selected actor (Outline)
		* @param p_actor
		* @param p_toStencil
		* @param p_isSelected
		*/
		void RenderActorOutlinePass(ActorPtr p_actor, bool p_toStencil, bool p_isSelected = false);

        /**
        * Render the camera perspective frustum
        * @param p_size
        * @param p_camera
        */
        void RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, LittleEngine::CCamera& p_camera);

        /**
        * Render the camera orthographic frustum
        * @param p_size
        * @param p_camera
        */
        void RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, LittleEngine::CCamera& p_camera);

		/**
		* Render the camera frustum
		*/
		void RenderCameraFrustum(LittleEngine::CCamera& p_camera);

		/**
		* Render an actor collider
		*/
		void RenderActorCollider(ActorPtr p_actor);

		/**
		* Render light bounds
		*/
		void RenderLightBounds(LittleEngine::CLight& p_light);

		/**
		* Render ambient box volume
		*/
		void RenderAmbientBoxVolume(LittleEngine::CAmbientBoxLight& p_ambientBoxLight);

		/**
		* Render ambient sphere volume
		*/
		void RenderAmbientSphereVolume(LittleEngine::CAmbientSphereLight& p_ambientSphereLight);

		/**
		* Render the the bounding spheres of the given model renderer
		*/
		void RenderBoundingSpheres(LittleEngine::CModelRenderer& p_modelRenderer);

		/**
		* Render model
		*/
		void RenderModelAsset(LittleEngine::Model& p_model);

		/**
		* Render texture
		*/
		void RenderTextureAsset(LittleEngine::Texture& p_texture);

		/**
		* Render material
		*/
		void RenderMaterialAsset(LittleEngine::Material& p_material);

		/**
		* Render the grid
		*/
		void RenderGrid(const LittleEngine::FVector3& p_viewPos, const LittleEngine::FVector3& p_color);

		/**
		* Update the light SSBO with the current scene
		* @param p_scene
		*/
		void UpdateLights(LittleEngine::Scene& p_scene);

		/**
		* Update the light SSBO with the current scene (Lights outside of the given frustum are culled)
		* @param p_scene
		*/
		void UpdateLightsInFrustum(LittleEngine::Scene& p_scene, const LittleEngine::Rendering::Data::Frustum& p_frustum);

	private:
		Context& m_context;

		LittleEngine::Material m_gridMaterial;
		LittleEngine::Material m_stencilFillMaterial;
		LittleEngine::Material m_textureMaterial;
		LittleEngine::Material m_outlineMaterial;
		LittleEngine::Material m_emptyMaterial;
		LittleEngine::Material m_defaultMaterial;
		LittleEngine::Material m_cameraMaterial;
		LittleEngine::Material m_lightMaterial;
		LittleEngine::Material m_gizmoArrowMaterial;
		LittleEngine::Material m_gizmoBallMaterial;
		LittleEngine::Material m_gizmoPickingMaterial;
		LittleEngine::Material m_actorPickingMaterial;
	};
}
