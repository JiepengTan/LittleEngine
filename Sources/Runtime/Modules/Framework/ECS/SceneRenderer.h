/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "map"

#include "Modules/Rendering/Core/Renderer.h"
#include "Resource/Asset/Mesh.h"
#include "Modules/Rendering/Data/Frustum.h"
#include "Modules/Rendering/Buffers/ShadowmapBuffer.h"


#include "Resource/Asset/Material.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/SceneSystem/Scene.h"

namespace LittleEngine
{
	/**
	* A Renderer capable of rendering stuffs linked with the ECS. It is a convenient class that should be used instead of Rendering::Core::Renderer
	* when you plan to use the LittleEngine ECS architecture.
	*/

	class SceneRenderer : public Rendering::Core::Renderer
	{
	public:
		class Drawable
		{
		public:
			FMatrix4 modelMatrix;
			Mesh* mesh;
			Material* material;
			FMatrix4 userMatrix;
			std::vector<FMatrix4>* boneMatrixAry;
			
		};
		using OpaqueDrawables		= std::multimap<float, Drawable, std::less<float>>;
		using TransparentDrawables	= std::multimap<float, Drawable, std::greater<float>>;

		/**
		* Constructor of the Renderer
		* @param p_driver
		*/
		SceneRenderer(Rendering::Context::Driver& p_driver);

		/**
		* Destructor of the Renderer
		* @param p_driver
		*/
		~SceneRenderer();

		/**
		* Parse a scene to find the main camera
		* @param p_scene
		*/
		SharedPtr<CCamera> FindMainCamera(Scene* p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information
		* @param p_scene
		*/
		std::vector<FMatrix4> FindLightMatrices(const Scene& p_scene);

		/**
		* Fill the given FMatrix4 vector with lights information that are inside the frustum
		* @param p_scene
		* @param p_frustum
		*/
		std::vector<FMatrix4> FindLightMatricesInFrustum(const Scene& p_scene, const Rendering::Data::Frustum& p_frustum);


		SharedPtr<CLight> FindMainLight(Scene& p_scene);
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
			Scene& p_scene,
			const FVector3& p_cameraPosition,
			const Rendering::LowRenderer::Camera& p_camera,
			const Rendering::Data::Frustum* p_customFrustum = nullptr,
			Material* p_defaultMaterial = nullptr
		);
		/**
			* Draw shadow map using the given scene 
			* @param p_scene
			* @param p_cameraPosition
			* @param p_camera
		*/
		void DrawShadowmap
		(
			Scene& p_scene,
			const FVector3& p_cameraPosition,
			const Rendering::LowRenderer::Camera& p_camera,
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
			const Scene& p_scene,
			const FVector3& p_cameraPosition,
			const Rendering::Data::Frustum& p_frustum,
			Material* p_defaultMaterial
		);

		/**
		* Returns opaque and transparents drawables from the scene
		* @param p_scene
		* @param p_cameraPosition
		* @param p_defaultMaterial
		*/
		std::pair<OpaqueDrawables, TransparentDrawables> FindAndSortDrawables
		(
			const Scene& p_scene,
			const FVector3& p_cameraPosition,
			Material* p_defaultMaterial
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
		void DrawModelWithSingleMaterial(Model& p_model, Material& p_material, FMatrix4 const* p_modelMatrix, Material* p_defaultMaterial = nullptr);

		/**
		* Draw the model using the given materials
		* @param p_model
		* @param p_modelMatrix
		* @param p_defaultMaterial (Used when a submesh material has no shader attached)
		*/
		void DrawModelWithMaterials(Model& p_model, std::vector<Material*> p_materials, FMatrix4 const* p_modelMatrix, Material* p_defaultMaterial = nullptr);

		/**
		* Try drawing a mesh using the given material (Fails if the material has no shader attached)
		* @param p_mesh
		* @param p_material
		* @param p_modelMatrix (If set to nullptr, no data will be sent to the GPU)
		*/
		void DrawMesh(Mesh& p_mesh, Material& p_material, FMatrix4 const* p_modelMatrix,std::vector<FMatrix4>* p_boneMatrixAry = nullptr);

		/**
		* Register the given function as the model matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the model matrix
		* to the GPU in the way you want
		* @param p_modelMatrixSender
		*/
		void RegisterModelMatrixSender(std::function<void(FMatrix4)> p_modelMatrixSender);

		/**
		* Register the given function as the user matrix sender.
		* The given function will be called before every draw calls.
		* You should use this function to send the user matrix
		* to the GPU in the way you want
		* @param p_userMatrixSender
		*/
		void RegisterUserMatrixSender(std::function<void(FMatrix4)> p_userMatrixSender);
		
	private:
		std::function<void(FMatrix4)> m_modelMatrixSender;
		std::function<void(FMatrix4)> m_userMatrixSender;
		std::unique_ptr<Rendering::Buffers::ShadowmapBuffer> m_shadowmapBuffer = nullptr;
		Texture* m_emptyTexture = nullptr;
		FMatrix4 m_lightSpaceVPMatrix;
		FVector4 m_lightInfo;
		bool m_isGPUSkin;
	};
}
