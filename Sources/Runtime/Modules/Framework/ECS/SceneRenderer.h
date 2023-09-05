/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>

#include "Modules/Rendering/Core/Renderer.h"
#include "Modules/Rendering/Resources/Mesh.h"
#include "Modules/Rendering/Data/Frustum.h"
#include "Modules/Rendering/Buffers/ShadowmapBuffer.h"


#include "Modules/Rendering/Resources/Material.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/SceneSystem/Scene.h"

namespace LittleEngine
{
	/**
	* A Renderer capable of rendering stuffs linked with the ECS. It is a convenient class that should be used instead of LittleEngine::Rendering::Core::Renderer
	* when you plan to use the LittleEngine ECS architecture.
	*/
	class SceneRenderer : public LittleEngine::Rendering::Core::Renderer
	{
	public:
		using Drawable				= std::tuple<LittleEngine::FMatrix4, LittleEngine::Rendering::Resources::Mesh*, LittleEngine::Resources::Material*, LittleEngine::FMatrix4, std::vector<LittleEngine::FMatrix4>*  >;
		using OpaqueDrawables		= std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables	= std::multimap<float, Drawable, std::greater<float>>;

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		SceneRenderer(LittleEngine::Rendering::Context::Driver& p_driver);

		/**
		* Destructor of the Renderer
		* @param p_driver
		*/
		~SceneRenderer();

		/**
		* Parse a scene to find the main camera
		* @param p_scene
		*/
		LittleEngine::CCamera* FindMainCamera(const LittleEngine::SceneSystem::Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information
		* @param p_scene
		*/
		std::vector<LittleEngine::FMatrix4> FindLightMatrices(const LittleEngine::SceneSystem::Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information that are inside the frustum
		* @param p_scene
		* @param p_frustum
		*/
		std::vector<LittleEngine::FMatrix4> FindLightMatricesInFrustum(const LittleEngine::SceneSystem::Scene& p_scene, const LittleEngine::Rendering::Data::Frustum& p_frustum);


		CLight* FindMainLight(const LittleEngine::SceneSystem::Scene& p_scene);
		/**
		* Draw the given scene using the given default material (optional) if no material found on an actor
		* @param p_scene
		* @param p_cameraPosition
		* @param p_camera
		* @param p_customFrustum
		* @param p_defaultMaterial
		*/
		void RenderScene
		(
			LittleEngine::SceneSystem::Scene& p_scene,
			const LittleEngine::FVector3& p_cameraPosition,
			const LittleEngine::Rendering::LowRenderer::Camera& p_camera,
			const LittleEngine::Rendering::Data::Frustum* p_customFrustum = nullptr,
			LittleEngine::Resources::Material* p_defaultMaterial = nullptr
		);
		/**
			* Draw shadow map using the given scene 
			* @param p_scene
			* @param p_cameraPosition
			* @param p_camera
		*/
		void DrawShadowmap
		(
			LittleEngine::SceneSystem::Scene& p_scene,
			const LittleEngine::FVector3& p_cameraPosition,
			const LittleEngine::Rendering::LowRenderer::Camera& p_camera,
			OpaqueDrawables&	opaqueMeshes
		);
		
		/**
		* Returns opaque and transparents drawables from the scene with frustum culling
		* @param p_scene
		* @param p_cameraPosition
		* @param p_frustum
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortFrustumCulledDrawables
		(
			const LittleEngine::SceneSystem::Scene& p_scene,
			const LittleEngine::FVector3& p_cameraPosition,
			const LittleEngine::Rendering::Data::Frustum& p_frustum,
			LittleEngine::Resources::Material* p_defaultMaterial
		);

		/**
		* Returns opaque and transparents drawables from the scene
		* @param p_scene
		* @param p_cameraPosition
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const LittleEngine::SceneSystem::Scene& p_scene,
			const LittleEngine::FVector3& p_cameraPosition,
			LittleEngine::Resources::Material* p_defaultMaterial
		);

		/**
		* Draw a Drawable instance
		* @param p_toDraw
		*/
		void DrawDrawable(const Drawable& p_toDraw);

		/**
		* Draw the model using the given material (The material will be applied to every submeshes of the the model)
		* @param p_model
		* @param p_material
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used if the given material has no shader attached)
		*/
		void DrawModelWithSingleMaterial(LittleEngine::Rendering::Resources::Model& p_model, LittleEngine::Resources::Material& p_material, LittleEngine::FMatrix4 const* p_modelMatrix, LittleEngine::Resources::Material* p_defaultMaterial = nullptr);

		/**
		* Draw the model using the given materials
		* @param p_model
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used when a submesh material has no shader attached)
		*/
		void DrawModelWithMaterials(LittleEngine::Rendering::Resources::Model& p_model, std::vector<LittleEngine::Resources::Material*> p_materials, LittleEngine::FMatrix4 const* p_modelMatrix, LittleEngine::Resources::Material* p_defaultMaterial = nullptr);

		/**
		* Try drawing a mesh using the given material (Fails if the material has no shader attached)
		* @param p_mesh
		* @param p_material
		* @param p_modelMatrix (If set to nullptr, no data will be sent to the GPU)
		*/
		void DrawMesh(LittleEngine::Rendering::Resources::Mesh& p_mesh, LittleEngine::Resources::Material& p_material, LittleEngine::FMatrix4 const* p_modelMatrix,std::vector<LittleEngine::FMatrix4>* p_boneMatrixAry = nullptr);

		/**
		* Register the given function as the model matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the model matrix
		* to the GPU in the way you want
		* @param p_modelMatrixSender
		*/
		void RegisterModelMatrixSender(std::function<void(LittleEngine::FMatrix4)> p_modelMatrixSender);

		/**
		* Register the given function as the user matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the user matrix
		* to the GPU in the way you want
		* @param p_userMatrixSender
		*/
		void RegisterUserMatrixSender(std::function<void(LittleEngine::FMatrix4)> p_userMatrixSender);
		
	private:
		std::function<void(LittleEngine::FMatrix4)> m_modelMatrixSender;
		std::function<void(LittleEngine::FMatrix4)> m_userMatrixSender;
		std::unique_ptr<LittleEngine::Rendering::Buffers::ShadowmapBuffer> m_shadowmapBuffer = nullptr;
		LittleEngine::Rendering::Resources::Texture* m_emptyTexture = nullptr;
		LittleEngine::FMatrix4 m_lightSpaceVPMatrix;
		LittleEngine::FVector4 m_lightInfo;
		bool m_isGPUSkin;
	};
}
