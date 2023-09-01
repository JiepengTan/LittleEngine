/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvRendering/Resources/Loaders/TextureLoader.h>
#include <OvRendering/Data/Frustum.h>

#include "OvCore/ECS/Renderer.h"

#include "OvCore/ECS/Components/CAnimator.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"
#include "OvCore/ResourceManagement/ShaderManager.h"
#include "OvCore/Global/ServiceLocator.h"
#include "OvCore/ResourceManagement/MaterialManager.h"
#include "OvCore/Helpers/RenderUtils.h"
#include "OvRendering/Buffers/Framebuffer.h"
#include "OvRendering/Resources/Mesh.h"


OvCore::ECS::Renderer::Renderer(OvRendering::Context::Driver& p_driver) :
	OvRendering::Core::Renderer(p_driver),
	m_emptyTexture(OvRendering::Resources::Loaders::TextureLoader::CreateColor
	(
		(255 << 24) | (255 << 16) | (255 << 8) | 255,
		OvRendering::Settings::ETextureFilteringMode::NEAREST,
		OvRendering::Settings::ETextureFilteringMode::NEAREST,
		false
	))
{
	m_shadowmapBuffer = std::make_unique<OvRendering::Buffers::ShadowmapBuffer>(1024,1024);
}

OvCore::ECS::Renderer::~Renderer()
{
	OvRendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

OvCore::ECS::Components::CCamera* OvCore::ECS::Renderer::FindMainCamera(const OvCore::SceneSystem::Scene& p_scene)
{
	for (OvCore::ECS::Components::CCamera* camera : p_scene.GetFastAccessComponents().cameras)
		if (camera->owner.IsActive())
			return camera;

	return nullptr;
}

std::vector<OvMaths::FMatrix4> OvCore::ECS::Renderer::FindLightMatrices(const OvCore::SceneSystem::Scene& p_scene)
{
	std::vector<OvMaths::FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner.IsActive())
		{
			result.push_back(light->GetData().GenerateMatrix());
		}
	}

	return result;
}



OvCore::ECS::Components::CLight* OvCore::ECS::Renderer::FindMainLight(const OvCore::SceneSystem::Scene& p_scene)
{
	const auto& facs = p_scene.GetFastAccessComponents();
	float maxIntersity = -1;
	ECS::Components::CLight* mainLight = nullptr;
	for (auto light : facs.lights)
	{
		if (light->owner.IsActive() && light->IsDirectional())
		{
			if(maxIntersity < light->GetIntensity())
			{
				maxIntersity = std::max(maxIntersity,light->GetIntensity());
				mainLight = light;
			}
		}
	}
	return mainLight;
}
std::vector<OvMaths::FMatrix4> OvCore::ECS::Renderer::FindLightMatricesInFrustum(const OvCore::SceneSystem::Scene& p_scene, const OvRendering::Data::Frustum& p_frustum)
{
	std::vector<OvMaths::FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner.IsActive())
		{
			const auto& lightData = light->GetData();
			const auto& position = lightData.GetTransform().GetWorldPosition();
			auto effectRange = lightData.GetEffectRange();

			// We always consider lights that have an +inf range (Not necessary to test if they are in frustum)
			if (std::isinf(effectRange) || p_frustum.SphereInFrustum(position.x, position.y, position.z, lightData.GetEffectRange()))
			{
				result.push_back(lightData.GenerateMatrix());
			}
		}
	}

	return result;
}
void OvCore::ECS::Renderer::DrawShadowmap
	(
		OvCore::SceneSystem::Scene& p_scene,
		const OvMaths::FVector3& p_cameraPosition,
		const OvRendering::LowRenderer::Camera& p_camera,
		OpaqueDrawables&	opaqueMeshes
	)
{
	int curFBO = OvRendering::Buffers::Framebuffer::m_curFrameBufferId;
	m_shadowmapBuffer->Bind();
	auto shadowMat = OvCore::Global::ServiceLocator::Get<OvCore::ResourceManagement::MaterialManager>()
		.GetResource(":Materials/Shadowmap.ovmat");
	auto mainLight = FindMainLight(p_scene);
	if(mainLight != nullptr)
	{
		
		//auto cameraMatrix = p_camera.GetProjectionMatrix();
		//auto cameraView = p_camera.GetViewMatrix();
		auto lightProject =  OvMaths::FMatrix4::CreateOrthographic(10, 1, 0.1, 100);
		auto lightTran = mainLight->owner.transform;
		auto pos = lightTran.GetWorldPosition();
		auto forward = lightTran.GetWorldForward();
		auto up = lightTran.GetWorldUp();
		// TODO Use Correct light pos to cover the enough scene
		auto lightRgihtPos =p_cameraPosition;
		auto lightView =  OvMaths::FMatrix4::CreateView(lightRgihtPos,	forward,up);
		m_lightSpaceVPMatrix = lightProject*lightView;
		m_lightInfo = OvMaths::FVector4(pos.x,pos.y,pos.z,1);
		uint8_t stateMask = shadowMat->GenerateStateMask();
		ApplyStateMask(stateMask);
		shadowMat->Bind(m_emptyTexture);
		shadowMat->GetShader()->SetUniformMat4("u_LightSpaceMatrix", m_lightSpaceVPMatrix	);
		for (const auto& [distance, drawable] : opaqueMeshes)
		{
			auto& modelMatrix = std::get<0>(drawable);
			shadowMat->GetShader()->SetUniformMat4("u_Local2World",	modelMatrix);
			auto mesh =std::get<1>(drawable);
			Draw(*mesh, OvRendering::Settings::EPrimitiveMode::TRIANGLES, 1);
		}
		shadowMat->UnBind();
	}
	m_shadowmapBuffer->Unbind();
	RecoverToLastViewPort();
	glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
}


void OvCore::ECS::Renderer::RenderScene
(
	OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	const OvRendering::LowRenderer::Camera& p_camera,
	const OvRendering::Data::Frustum* p_customFrustum,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (p_camera.HasFrustumGeometryCulling())
	{
		const auto& frustum = p_customFrustum ? *p_customFrustum : p_camera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortFrustumCulledDrawables(p_scene, p_cameraPosition, frustum, p_defaultMaterial);
	}
	else
	{
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(p_scene, p_cameraPosition, p_defaultMaterial);
	}

	// render shadow
	
	DrawShadowmap(p_scene,p_cameraPosition,p_camera,opaqueMeshes);
	{
		for (const auto& [distance, drawable] : opaqueMeshes)
			DrawDrawable(drawable);

		for (const auto& [distance, drawable] : transparentMeshes)
			DrawDrawable(drawable);
	}

	/*
	if(p_camera.m_CameraType == OvRendering::Settings::ECameraType ::Game)
		RenderUtils::DrawDebugQuad(m_shadowmapBuffer->GetTexture());
	*/
}

std::vector<OvMaths::FMatrix4>* GetBoneMatrix(OvCore::ECS::Components::CModelRenderer* modelRenderer)
{
	auto model = modelRenderer->GetModel();
	if(model && model->isSkinMesh)
	{
		auto anim = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CAnimator>();
		if(anim != nullptr)
		{
			return anim->GetFinalBoneMatrices();
		}
	}
	return nullptr;
}

void FindAndSortDrawables
(
	OvCore::ECS::Renderer::OpaqueDrawables& p_opaques,
	OvCore::ECS::Renderer::TransparentDrawables& p_transparents,
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	for (OvCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = OvMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);
				if (auto materialRenderer = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetFTransform();

					const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					std::vector<OvMaths::FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
					for (auto mesh : model->GetMeshes())
					{
						OvCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							OvCore::ECS::Renderer::Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix(),boneAryPtr };

							if (material->IsBlendable())
								p_transparents.emplace(distanceToActor, element);
							else
								p_opaques.emplace(distanceToActor, element);
						}
					}
				}
			}
		}
	}
}

std::pair<OvCore::ECS::Renderer::OpaqueDrawables, OvCore::ECS::Renderer::TransparentDrawables> OvCore::ECS::Renderer::FindAndSortFrustumCulledDrawables
(
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	const OvRendering::Data::Frustum& p_frustum,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	using namespace OvCore::ECS::Components;

	OvCore::ECS::Renderer::OpaqueDrawables opaqueDrawables;
	OvCore::ECS::Renderer::TransparentDrawables transparentDrawables;

	for (CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		auto& owner = modelRenderer->owner;

		if (owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner.GetComponent<CMaterialRenderer>())
				{
					auto& transform = owner.transform.GetFTransform();

					OvRendering::Settings::ECullingOptions cullingOptions = OvRendering::Settings::ECullingOptions::NONE;

					if (modelRenderer->GetFrustumBehaviour() != CModelRenderer::EFrustumBehaviour::DISABLED)
					{
						cullingOptions |= OvRendering::Settings::ECullingOptions::FRUSTUM_PER_MODEL;
					}

					if (modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_MESHES)
					{
						cullingOptions |= OvRendering::Settings::ECullingOptions::FRUSTUM_PER_MESH;
					}

					const auto& modelBoundingSphere = modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ? modelRenderer->GetCustomBoundingSphere() : model->GetBoundingSphere();

					std::vector<std::reference_wrapper<OvRendering::Resources::Mesh>> meshes;

					{
						PROFILER_SPY("Frustum Culling");
						meshes = GetMeshesInFrustum(*model, modelBoundingSphere, transform, p_frustum, cullingOptions);
					}

					if (!meshes.empty())
					{
						float distanceToActor = OvMaths::FVector3::Distance(transform.GetWorldPosition(), p_cameraPosition);
						const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						std::vector<OvMaths::FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
						for (const auto& mesh : meshes)
						{
							OvCore::Resources::Material* material = nullptr;

							if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh.get().GetMaterialIndex());
								if (!material || !material->GetShader())
									material = p_defaultMaterial;
							}

							if (material)
							{
								OvCore::ECS::Renderer::Drawable element = { transform.GetWorldMatrix(), &mesh.get(), material, materialRenderer->GetUserMatrix() ,boneAryPtr};

								if (material->IsBlendable())
									transparentDrawables.emplace(distanceToActor, element);
								else
									opaqueDrawables.emplace(distanceToActor, element);
							}
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

std::pair<OvCore::ECS::Renderer::OpaqueDrawables, OvCore::ECS::Renderer::TransparentDrawables> OvCore::ECS::Renderer::FindAndSortDrawables
(
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	OvCore::ECS::Renderer::OpaqueDrawables opaqueDrawables;
	OvCore::ECS::Renderer::TransparentDrawables transparentDrawables;

	for (OvCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = OvMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

				if (auto materialRenderer = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetFTransform();

					const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					std::vector<OvMaths::FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
					for (auto mesh : model->GetMeshes())
					{
						OvCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							OvCore::ECS::Renderer::Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix(),boneAryPtr };

							if (material->IsBlendable())
								transparentDrawables.emplace(distanceToActor, element);
							else
								opaqueDrawables.emplace(distanceToActor, element);
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

void OvCore::ECS::Renderer::DrawDrawable(const Drawable& p_toDraw)
{
	m_userMatrixSender(std::get<3>(p_toDraw));
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw),std::get<4>(p_toDraw));
}

void OvCore::ECS::Renderer::DrawModelWithSingleMaterial(OvRendering::Resources::Model& p_model, OvCore::Resources::Material& p_material, OvMaths::FMatrix4 const* p_modelMatrix, OvCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		OvCore::Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial;

		if (material)
			DrawMesh(*mesh, *material, nullptr,nullptr);
	}
}

void OvCore::ECS::Renderer::DrawModelWithMaterials(OvRendering::Resources::Model& p_model, std::vector<OvCore::Resources::Material*> p_materials, OvMaths::FMatrix4 const* p_modelMatrix, OvCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		OvCore::Resources::Material* material = p_materials.size() > mesh->GetMaterialIndex() ? p_materials[mesh->GetMaterialIndex()] : p_defaultMaterial;
		if (material)
			DrawMesh(*mesh, *material, nullptr,nullptr);
	}
}

void OvCore::ECS::Renderer::DrawMesh(OvRendering::Resources::Mesh& p_mesh, OvCore::Resources::Material& p_material, OvMaths::FMatrix4 const* p_modelMatrix,
std::vector<OvMaths::FMatrix4>* p_boneMatrixAry
)
{
	using namespace OvRendering::Settings;
	p_material.Set("u_Shadowmap",m_shadowmapBuffer->GetTexture(),true);
	if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
			m_modelMatrixSender(*p_modelMatrix);

		uint8_t stateMask = p_material.GenerateStateMask();
		ApplyStateMask(stateMask);
		
		/* Draw the mesh */
		p_material.Bind(m_emptyTexture);
		auto shader = p_material.GetShader();
		shader->SetUniformMat4("u_LightSpaceMatrix",m_lightSpaceVPMatrix );
		shader->SetUniformVec4("u_ShadowLightPosition", m_lightInfo);
		shader->SetUniformInt("u_IsSkinMesh", (p_mesh.isSkinMesh?1:0));
		if(p_mesh.isSkinMesh && p_boneMatrixAry != nullptr )
		{
			shader->SetUniformMat4Array("u_BonesMatrices",*p_boneMatrixAry);
		}
		
		Draw(p_mesh, OvRendering::Settings::EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.UnBind();
	}
}

void OvCore::ECS::Renderer::RegisterModelMatrixSender(std::function<void(OvMaths::FMatrix4)> p_modelMatrixSender)
{
	m_modelMatrixSender = p_modelMatrixSender;
}

void OvCore::ECS::Renderer::RegisterUserMatrixSender(std::function<void(OvMaths::FMatrix4)> p_userMatrixSender)
{
	m_userMatrixSender = p_userMatrixSender;
}
