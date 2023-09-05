/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Analytics/Profiling/ProfilerSpy.h"

#include "Modules/Rendering/Resources/Loaders/TextureLoader.h"
#include "Modules/Rendering/Data/Frustum.h"

#include "Modules/Framework/ECS/SceneRenderer.h"

#include "Modules/Framework/ECS/Components/CAnimator.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Utils/RenderUtils.h"
#include "Modules/Rendering/Buffers/Framebuffer.h"
#include "Modules/Rendering/Resources/Mesh.h"


LittleEngine::SceneRenderer::SceneRenderer(LittleEngine::Rendering::Context::Driver& p_driver) :
	LittleEngine::Rendering::Core::Renderer(p_driver),m_isGPUSkin(false),
	m_emptyTexture(LittleEngine::Rendering::Resources::Loaders::TextureLoader::CreateColor
	(
		(255 << 24) | (255 << 16) | (255 << 8) | 255,
		LittleEngine::Rendering::Settings::ETextureFilteringMode::NEAREST,
		LittleEngine::Rendering::Settings::ETextureFilteringMode::NEAREST,
		false
	))
{
	m_shadowmapBuffer = std::make_unique<LittleEngine::Rendering::Buffers::ShadowmapBuffer>(2048,2048);
}

LittleEngine::SceneRenderer::~SceneRenderer()
{
	LittleEngine::Rendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

LittleEngine::CCamera* LittleEngine::SceneRenderer::FindMainCamera(const LittleEngine::SceneSystem::Scene& p_scene)
{
	for (LittleEngine::CCamera* camera : p_scene.GetFastAccessComponents().cameras)
		if (camera->owner->IsActive())
			return camera;

	return nullptr;
}

std::vector<LittleEngine::FMatrix4> LittleEngine::SceneRenderer::FindLightMatrices(const LittleEngine::SceneSystem::Scene& p_scene)
{
	std::vector<LittleEngine::FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner->IsActive())
		{
			result.push_back(light->GetData().GenerateMatrix());
		}
	}

	return result;
}



LittleEngine::CLight* LittleEngine::SceneRenderer::FindMainLight(const LittleEngine::SceneSystem::Scene& p_scene)
{
	const auto& facs = p_scene.GetFastAccessComponents();
	float maxIntersity = -1;
	CLight* mainLight = nullptr;
	for (auto light : facs.lights)
	{
		if (light->owner->IsActive() && light->IsDirectional())
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
std::vector<LittleEngine::FMatrix4> LittleEngine::SceneRenderer::FindLightMatricesInFrustum(const LittleEngine::SceneSystem::Scene& p_scene, const LittleEngine::Rendering::Data::Frustum& p_frustum)
{
	std::vector<LittleEngine::FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner->IsActive())
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
void LittleEngine::SceneRenderer::DrawShadowmap
	(
		LittleEngine::SceneSystem::Scene& p_scene,
		const LittleEngine::FVector3& p_cameraPosition,
		const LittleEngine::Rendering::LowRenderer::Camera& p_camera,
		OpaqueDrawables&	opaqueMeshes
	)
{
	PROFILER_SPY("DrawShadowmap");
	int curFBO = LittleEngine::Rendering::Buffers::Framebuffer::m_curFrameBufferId;
	m_shadowmapBuffer->Bind();
	auto shadowMat = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::MaterialManager>()
		.GetResource(":Materials/Shadowmap.ovmat");
	auto mainLight = FindMainLight(p_scene);
	if(mainLight != nullptr)
	{
		
		//auto cameraMatrix = p_camera.GetProjectionMatrix();
		//auto cameraView = p_camera.GetViewMatrix();
		auto lightProject =  LittleEngine::FMatrix4::CreateOrthographic(10, 1, 0.1f, 100);
		auto lightTran = mainLight->owner->transform;
		auto pos = lightTran.GetWorldPosition();
		auto forward = lightTran.GetWorldForward();
		auto up = lightTran.GetWorldUp();
		// TODO Use Correct light pos to cover the enough scene
		auto lightRgihtPos =p_cameraPosition;
		auto lightView =  LittleEngine::FMatrix4::CreateView(lightRgihtPos,	forward,up);
		m_lightSpaceVPMatrix = lightProject*lightView;
		m_lightInfo = LittleEngine::FVector4(pos.x,pos.y,pos.z,1);
		uint8_t stateMask = shadowMat->GenerateStateMask();
		ApplyStateMask(stateMask);
		shadowMat->Bind(m_emptyTexture);
		shadowMat->GetShader()->SetUniformMat4("u_LightSpaceMatrix", m_lightSpaceVPMatrix	);
		for (const auto& [distance, drawable] : opaqueMeshes)
		{
			auto& modelMatrix = std::get<0>(drawable);
			shadowMat->GetShader()->SetUniformMat4("u_Local2World",	modelMatrix);
			auto mesh =std::get<1>(drawable);
			Draw(*mesh, LittleEngine::Rendering::Settings::EPrimitiveMode::TRIANGLES, 1);
		}
		shadowMat->UnBind();
	}
	m_shadowmapBuffer->Unbind();
	RecoverToLastViewPort();
	glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
}


void LittleEngine::SceneRenderer::RenderScene
(
	LittleEngine::SceneSystem::Scene& p_scene,
	const LittleEngine::FVector3& p_cameraPosition,
	const LittleEngine::Rendering::LowRenderer::Camera& p_camera,
	const LittleEngine::Rendering::Data::Frustum* p_customFrustum,
	LittleEngine::Resources::Material* p_defaultMaterial
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
	if(p_camera.m_CameraType == LittleEngine::Rendering::Settings::ECameraType ::Game)
		RenderUtils::DrawDebugQuad(m_shadowmapBuffer->GetTexture());
	*/
}

std::vector<LittleEngine::FMatrix4>* GetBoneMatrix(LittleEngine::CModelRenderer* modelRenderer)
{
	auto model = modelRenderer->GetModel();
	if(model && model->isSkinMesh)
	{
		auto anim = modelRenderer->owner->GetComponent<LittleEngine::CAnimator>();
		if(anim != nullptr)
		{
			return anim->GetFinalBoneMatrices();
		}
	}
	return nullptr;
}

void FindAndSortDrawables
(
	LittleEngine::SceneRenderer::OpaqueDrawables& p_opaques,
	LittleEngine::SceneRenderer::TransparentDrawables& p_transparents,
	const LittleEngine::SceneSystem::Scene& p_scene,
	const LittleEngine::FVector3& p_cameraPosition,
	LittleEngine::Resources::Material* p_defaultMaterial
)
{
	for (LittleEngine::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner->IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = LittleEngine::FVector3::Distance(modelRenderer->owner->transform.GetWorldPosition(), p_cameraPosition);
				if (auto materialRenderer = modelRenderer->owner->GetComponent<LittleEngine::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner->transform.GetFTransform();

					const LittleEngine::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					std::vector<LittleEngine::FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
					for (auto mesh : model->GetMeshes())
					{
						LittleEngine::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							LittleEngine::SceneRenderer::Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix(),boneAryPtr };

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

std::pair<LittleEngine::SceneRenderer::OpaqueDrawables, LittleEngine::SceneRenderer::TransparentDrawables> LittleEngine::SceneRenderer::FindAndSortFrustumCulledDrawables
(
	const LittleEngine::SceneSystem::Scene& p_scene,
	const LittleEngine::FVector3& p_cameraPosition,
	const LittleEngine::Rendering::Data::Frustum& p_frustum,
	LittleEngine::Resources::Material* p_defaultMaterial
)
{
	using namespace LittleEngine;

	LittleEngine::SceneRenderer::OpaqueDrawables opaqueDrawables;
	LittleEngine::SceneRenderer::TransparentDrawables transparentDrawables;

	for (CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		auto actor = modelRenderer->owner;

		if (actor->IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner->GetComponent<CMaterialRenderer>())
				{
					auto& transform = actor->transform.GetFTransform();

					LittleEngine::Rendering::Settings::ECullingOptions cullingOptions = LittleEngine::Rendering::Settings::ECullingOptions::NONE;

					if (modelRenderer->GetFrustumBehaviour() != CModelRenderer::EFrustumBehaviour::DISABLED)
					{
						cullingOptions |= LittleEngine::Rendering::Settings::ECullingOptions::FRUSTUM_PER_MODEL;
					}

					if (modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_MESHES)
					{
						cullingOptions |= LittleEngine::Rendering::Settings::ECullingOptions::FRUSTUM_PER_MESH;
					}

					const auto& modelBoundingSphere = modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ? modelRenderer->GetCustomBoundingSphere() : model->GetBoundingSphere();

					std::vector<std::reference_wrapper<LittleEngine::Rendering::Resources::Mesh>> meshes;

					{
						PROFILER_SPY("Frustum Culling");
						meshes = GetMeshesInFrustum(*model, modelBoundingSphere, transform, p_frustum, cullingOptions);
					}

					if (!meshes.empty())
					{
						float distanceToActor = LittleEngine::FVector3::Distance(transform.GetWorldPosition(), p_cameraPosition);
						const LittleEngine::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						std::vector<LittleEngine::FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
						for (const auto& mesh : meshes)
						{
							LittleEngine::Resources::Material* material = nullptr;

							if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh.get().GetMaterialIndex());
								if (!material || !material->GetShader())
									material = p_defaultMaterial;
							}

							if (material)
							{
								LittleEngine::SceneRenderer::Drawable element = { transform.GetWorldMatrix(), &mesh.get(), material, materialRenderer->GetUserMatrix() ,boneAryPtr};

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

std::pair<LittleEngine::SceneRenderer::OpaqueDrawables, LittleEngine::SceneRenderer::TransparentDrawables> LittleEngine::SceneRenderer::FindAndSortDrawables
(
	const LittleEngine::SceneSystem::Scene& p_scene,
	const LittleEngine::FVector3& p_cameraPosition,
	LittleEngine::Resources::Material* p_defaultMaterial
)
{
	LittleEngine::SceneRenderer::OpaqueDrawables opaqueDrawables;
	LittleEngine::SceneRenderer::TransparentDrawables transparentDrawables;

	for (LittleEngine::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner->IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = LittleEngine::FVector3::Distance(modelRenderer->owner->transform.GetWorldPosition(), p_cameraPosition);

				if (auto materialRenderer = modelRenderer->owner->GetComponent<LittleEngine::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner->transform.GetFTransform();

					const LittleEngine::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					std::vector<LittleEngine::FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
					for (auto mesh : model->GetMeshes())
					{
						LittleEngine::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
						}

						if (material)
						{
							LittleEngine::SceneRenderer::Drawable element = { transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix(),boneAryPtr };

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

void LittleEngine::SceneRenderer::DrawDrawable(const Drawable& p_toDraw)
{
	m_userMatrixSender(std::get<3>(p_toDraw));
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw),std::get<4>(p_toDraw));
}

void LittleEngine::SceneRenderer::DrawModelWithSingleMaterial(LittleEngine::Rendering::Resources::Model& p_model, LittleEngine::Resources::Material& p_material, LittleEngine::FMatrix4 const* p_modelMatrix, LittleEngine::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		LittleEngine::Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial;

		if (material)
			DrawMesh(*mesh, *material, nullptr,nullptr);
	}
}

void LittleEngine::SceneRenderer::DrawModelWithMaterials(LittleEngine::Rendering::Resources::Model& p_model, std::vector<LittleEngine::Resources::Material*> p_materials, LittleEngine::FMatrix4 const* p_modelMatrix, LittleEngine::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		LittleEngine::Resources::Material* material = p_materials.size() > mesh->GetMaterialIndex() ? p_materials[mesh->GetMaterialIndex()] : p_defaultMaterial;
		if (material)
			DrawMesh(*mesh, *material, nullptr,nullptr);
	}
}

void LittleEngine::SceneRenderer::DrawMesh(LittleEngine::Rendering::Resources::Mesh& p_mesh, LittleEngine::Resources::Material& p_material, LittleEngine::FMatrix4 const* p_modelMatrix,
std::vector<LittleEngine::FMatrix4>* p_boneMatrixAry
)
{
	using namespace LittleEngine::Rendering::Settings;
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
		if(m_isGPUSkin)
		{
			shader->SetUniformInt("u_IsSkinMesh", (p_mesh.isSkinMesh?1:0));
			if(p_mesh.isSkinMesh && p_boneMatrixAry != nullptr )
			{
				shader->SetUniformMat4Array("u_BonesMatrices",*p_boneMatrixAry);
			}
		}
		Draw(p_mesh, LittleEngine::Rendering::Settings::EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.UnBind();
	}
}

void LittleEngine::SceneRenderer::RegisterModelMatrixSender(std::function<void(LittleEngine::FMatrix4)> p_modelMatrixSender)
{
	m_modelMatrixSender = p_modelMatrixSender;
}

void LittleEngine::SceneRenderer::RegisterUserMatrixSender(std::function<void(LittleEngine::FMatrix4)> p_userMatrixSender)
{
	m_userMatrixSender = p_userMatrixSender;
}
