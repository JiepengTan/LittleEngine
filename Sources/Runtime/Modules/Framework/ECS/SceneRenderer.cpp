/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Analytics/Profiling/ProfilerSpy.h"

#include "Resource/Loader/TextureLoader.h"
#include "Modules/Rendering/Data/Frustum.h"

#include "Modules/Framework/ECS/SceneRenderer.h"

#include "Components/CLight.h"
#include "Modules/Framework/ECS/Components/CAnimator.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Resource/Manager/ShaderManager.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/MaterialManager.h"
#include "Modules/Utils/RenderUtils.h"
#include "Modules/Rendering/Buffers/Framebuffer.h"
#include "Resource/Asset/Mesh.h"

namespace LittleEngine
{
    SceneRenderer::SceneRenderer(Rendering::Context::Driver& p_driver) :
        Rendering::Core::Renderer(p_driver), m_isGPUSkin(false),
        m_emptyTexture(Rendering::Resources::Loaders::TextureLoader::CreateColor
            (
                (255 << 24) | (255 << 16) | (255 << 8) | 255,
                Rendering::Settings::ETextureFilteringMode::NEAREST,
                Rendering::Settings::ETextureFilteringMode::NEAREST,
                false
            ))
    {
        m_shadowmapBuffer = std::make_unique<Rendering::Buffers::ShadowmapBuffer>(2048, 2048);
    }

    SceneRenderer::~SceneRenderer()
    {
        Rendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
    }

    SharedPtr<CCamera> SceneRenderer::FindMainCamera(Scene* p_scene)
    {
        auto cameras = p_scene->GetFastAccessComponents().cameras;
        for (auto cameraId : cameras)
        {
            auto camera = p_scene->GetActor(cameraId);
            if (camera != nullptr && camera->IsActive())
                return camera->GetComponent<CCamera>();

        }
        return nullptr;
    }

    std::vector<FMatrix4> SceneRenderer::FindLightMatrices(
        const Scene& p_scene)
    {
        std::vector<FMatrix4> result;

        for (auto light : p_scene.GetLights())
        {
            if (light->GetActor()->IsActive())
            {
                result.push_back(light->GetData().GenerateMatrix());
            }
        }

        return result;
    }


    SharedPtr<CLight> SceneRenderer::FindMainLight(Scene& p_scene)
    {
        const auto& facs = p_scene.GetFastAccessComponents();
        float maxIntersity = -1;
        SharedPtr<CLight> mainLight = nullptr;
        for (auto light : p_scene.GetLights())
        {
            if (light->GetActor()->IsActive() && light->IsDirectional())
            {
                if (maxIntersity < light->GetIntensity())
                {
                    maxIntersity = std::max(maxIntersity, light->GetIntensity());
                    mainLight = light;
                }
            }
        }
        return mainLight;
    }

    std::vector<FMatrix4> SceneRenderer::FindLightMatricesInFrustum(
        const Scene& p_scene, const Rendering::Data::Frustum& p_frustum)
    {
        std::vector<FMatrix4> result;
        for (auto light : p_scene.GetLights())
        {
            if (light->GetActor()->IsActive())
            {
                const auto& lightData = light->GetData();
                const auto& position = lightData.GetTransform()->GetWorldPosition();
                auto effectRange = lightData.GetEffectRange();

                // We always consider lights that have an +inf range (Not necessary to test if they are in frustum)
                if (std::isinf(effectRange) || p_frustum.SphereInFrustum(
                    position.x, position.y, position.z, lightData.GetEffectRange()))
                {
                    result.push_back(lightData.GenerateMatrix());
                }
            }
        }

        return result;
    }

    void SceneRenderer::DrawShadowmap
    (
        Scene& p_scene,
        const FVector3& p_cameraPosition,
        const Rendering::LowRenderer::Camera& p_camera,
        OpaqueDrawables& opaqueMeshes
    )
    {
        PROFILER_SPY("DrawShadowmap");
        int curFBO = Rendering::Buffers::Framebuffer::m_curFrameBufferId;
        m_shadowmapBuffer->Bind();
        auto shadowMat = Global::ServiceLocator::Get<
                ResourceManagement::MaterialManager>()
            .GetResource(":Materials/Shadowmap.ovmat");
        auto mainLight = FindMainLight(p_scene);
        if (mainLight != nullptr)
        {
            //auto cameraMatrix = p_camera.GetProjectionMatrix();
            //auto cameraView = p_camera.GetViewMatrix();
            auto lightProject = FMatrix4::CreateOrthographic(10, 1, 0.1f, 100);
            auto lightTran = mainLight->GetActor()->transform;
            auto pos = lightTran->GetWorldPosition();
            auto forward = lightTran->GetWorldForward();
            auto up = lightTran->GetWorldUp();
            // TODO Use Correct light pos to cover the enough scene
            auto lightRgihtPos = p_cameraPosition;
            auto lightView = FMatrix4::CreateView(lightRgihtPos, forward, up);
            m_lightSpaceVPMatrix = lightProject * lightView;
            m_lightInfo = FVector4(pos.x, pos.y, pos.z, 1);
            uint8_t stateMask = shadowMat->GenerateStateMask();
            ApplyStateMask(stateMask);
            shadowMat->Bind(m_emptyTexture);
            shadowMat->GetShader()->SetUniformMat4("u_LightSpaceMatrix", m_lightSpaceVPMatrix);
            for (const auto& [distance, drawable] : opaqueMeshes)
            {
                auto& modelMatrix = std::get<0>(drawable);
                shadowMat->GetShader()->SetUniformMat4("u_Local2World", modelMatrix);
                auto mesh = std::get<1>(drawable);
                Draw(*mesh, Rendering::Settings::EPrimitiveMode::TRIANGLES, 1);
            }
            shadowMat->UnBind();
        }
        m_shadowmapBuffer->Unbind();
        RecoverToLastViewPort();
        glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
    }


    void SceneRenderer::RenderScene
    (
        Scene& p_scene,
        const FVector3& p_cameraPosition,
        const Rendering::LowRenderer::Camera& p_camera,
        const Rendering::Data::Frustum* p_customFrustum,
        Resources::Material* p_defaultMaterial
    )
    {
        OpaqueDrawables opaqueMeshes;
        TransparentDrawables transparentMeshes;

        if (p_camera.HasFrustumGeometryCulling())
        {
            const auto& frustum = p_customFrustum ? *p_customFrustum : p_camera.GetFrustum();
            std::tie(opaqueMeshes, transparentMeshes) = FindAndSortFrustumCulledDrawables(
                p_scene, p_cameraPosition, frustum, p_defaultMaterial);
        }
        else
        {
            std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(
                p_scene, p_cameraPosition, p_defaultMaterial);
        }

        // render shadow

        DrawShadowmap(p_scene, p_cameraPosition, p_camera, opaqueMeshes);
        {
            for (const auto& [distance, drawable] : opaqueMeshes)
                DrawDrawable(drawable);

            for (const auto& [distance, drawable] : transparentMeshes)
                DrawDrawable(drawable);
        }

        /*
        if(p_camera.m_CameraType == Rendering::Settings::ECameraType ::Game)
            RenderUtils::DrawDebugQuad(m_shadowmapBuffer->GetTexture());
        */
    }

    std::vector<FMatrix4>* GetBoneMatrix(SharedPtr<CModelRenderer> modelRenderer)
    {
        auto model = modelRenderer->GetModel();
        if (model && model->isSkinMesh)
        {
            auto anim = modelRenderer->GetActor()->GetComponent<CAnimator>();
            if (anim != nullptr)
            {
                return anim->GetFinalBoneMatrices();
            }
        }
        return nullptr;
    }

    void FindAndSortDrawables
    (
        SceneRenderer::OpaqueDrawables& p_opaques,
        SceneRenderer::TransparentDrawables& p_transparents,
        const Scene& p_scene,
        const FVector3& p_cameraPosition,
        Resources::Material* p_defaultMaterial
    )
    {
        for (SharedPtr<CModelRenderer> modelRenderer : p_scene.GetRenderers())
        {
            if (modelRenderer->GetActor()->IsActive())
            {
                if (auto model = modelRenderer->GetModel())
                {
                    float distanceToActor = FVector3::Distance(
                        modelRenderer->GetActor()->transform->GetWorldPosition(), p_cameraPosition);
                    if (auto materialRenderer = modelRenderer->GetActor()->GetComponent<
                        CMaterialRenderer>())
                    {
                        const auto transform = modelRenderer->GetActor()->transform->GetFTransform();

                        const CMaterialRenderer::MaterialList& materials = materialRenderer->
                            GetMaterials();

                        std::vector<FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
                        for (auto mesh : model->GetMeshes())
                        {
                            Resources::Material* material = nullptr;

                            if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
                            {
                                material = materials.at(mesh->GetMaterialIndex());
                                if (!material || !material->GetShader())
                                    material = p_defaultMaterial;
                            }

                            if (material)
                            {
                                SceneRenderer::Drawable element = {
                                    transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix(),
                                    boneAryPtr
                                };

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

    std::pair<SceneRenderer::OpaqueDrawables, SceneRenderer::TransparentDrawables>
    SceneRenderer::FindAndSortFrustumCulledDrawables
    (
        const Scene& p_scene,
        const FVector3& p_cameraPosition,
        const Rendering::Data::Frustum& p_frustum,
        Resources::Material* p_defaultMaterial
    )
    {
        using namespace LittleEngine;

        SceneRenderer::OpaqueDrawables opaqueDrawables;
        SceneRenderer::TransparentDrawables transparentDrawables;

        for (SharedPtr<CModelRenderer> modelRenderer : p_scene.GetRenderers())
        {
            auto actor = modelRenderer->GetActor();

            if (actor->IsActive())
            {
                if (auto model = modelRenderer->GetModel())
                {
                    if (auto materialRenderer = modelRenderer->GetActor()->GetComponent<CMaterialRenderer>())
                    {
                        auto& transform = actor->transform->GetFTransform();

                        Rendering::Settings::ECullingOptions cullingOptions =
                            Rendering::Settings::ECullingOptions::NONE;

                        if (modelRenderer->GetFrustumBehaviour() != CModelRenderer::EFrustumBehaviour::DISABLED)
                        {
                            cullingOptions |= Rendering::Settings::ECullingOptions::FRUSTUM_PER_MODEL;
                        }

                        if (modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_MESHES)
                        {
                            cullingOptions |= Rendering::Settings::ECullingOptions::FRUSTUM_PER_MESH;
                        }

                        const auto& modelBoundingSphere =
                            modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_CUSTOM
                                ? modelRenderer->GetCustomBoundingSphere()
                                : model->GetBoundingSphere();

                        std::vector<std::reference_wrapper<Rendering::Resources::Mesh>> meshes;

                        {
                            PROFILER_SPY("Frustum Culling");
                            meshes = GetMeshesInFrustum(*model, modelBoundingSphere, transform, p_frustum,
                                                        cullingOptions);
                        }

                        if (!meshes.empty())
                        {
                            float distanceToActor = FVector3::Distance(
                                transform.GetWorldPosition(), p_cameraPosition);
                            const CMaterialRenderer::MaterialList& materials = materialRenderer->
                                GetMaterials();

                            std::vector<FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
                            for (const auto& mesh : meshes)
                            {
                                Resources::Material* material = nullptr;

                                if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
                                {
                                    material = materials.at(mesh.get().GetMaterialIndex());
                                    if (!material || !material->GetShader())
                                        material = p_defaultMaterial;
                                }

                                if (material)
                                {
                                    SceneRenderer::Drawable element = {
                                        transform.GetWorldMatrix(), &mesh.get(), material,
                                        materialRenderer->GetUserMatrix(), boneAryPtr
                                    };

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

        return {opaqueDrawables, transparentDrawables};
    }

    std::pair<SceneRenderer::OpaqueDrawables, SceneRenderer::TransparentDrawables>
    SceneRenderer::FindAndSortDrawables
    (
        const Scene& p_scene,
        const FVector3& p_cameraPosition,
        Resources::Material* p_defaultMaterial
    )
    {
        SceneRenderer::OpaqueDrawables opaqueDrawables;
        SceneRenderer::TransparentDrawables transparentDrawables;

        for (SharedPtr<CModelRenderer> modelRenderer : p_scene.GetRenderers())
        {
            if (modelRenderer->GetActor()->IsActive())
            {
                if (auto model = modelRenderer->GetModel())
                {
                    float distanceToActor = FVector3::Distance(
                        modelRenderer->GetActor()->transform->GetWorldPosition(), p_cameraPosition);

                    if (auto materialRenderer = modelRenderer->GetActor()->GetComponent<
                        CMaterialRenderer>())
                    {
                        const auto& transform = modelRenderer->GetActor()->transform->GetFTransform();

                        const CMaterialRenderer::MaterialList& materials = materialRenderer->
                            GetMaterials();

                        std::vector<FMatrix4>* boneAryPtr = GetBoneMatrix(modelRenderer);
                        for (auto mesh : model->GetMeshes())
                        {
                            Resources::Material* material = nullptr;

                            if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
                            {
                                material = materials.at(mesh->GetMaterialIndex());
                                if (!material || !material->GetShader())
                                    material = p_defaultMaterial;
                            }

                            if (material)
                            {
                                SceneRenderer::Drawable element = {
                                    transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix(),
                                    boneAryPtr
                                };

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

        return {opaqueDrawables, transparentDrawables};
    }

    void SceneRenderer::DrawDrawable(const Drawable& p_toDraw)
    {
        m_userMatrixSender(std::get<3>(p_toDraw));
        DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw), std::get<4>(p_toDraw));
    }

    void SceneRenderer::DrawModelWithSingleMaterial(
        Rendering::Resources::Model& p_model, Resources::Material& p_material,
        FMatrix4 const* p_modelMatrix, Resources::Material* p_defaultMaterial)
    {
        if (p_modelMatrix)
            m_modelMatrixSender(*p_modelMatrix);

        for (auto mesh : p_model.GetMeshes())
        {
            Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial;

            if (material)
                DrawMesh(*mesh, *material, nullptr, nullptr);
        }
    }

    void SceneRenderer::DrawModelWithMaterials(Rendering::Resources::Model& p_model,
                                               std::vector<Resources::Material*>
                                               p_materials,
                                               FMatrix4 const* p_modelMatrix,
                                               Resources::Material* p_defaultMaterial)
    {
        if (p_modelMatrix)
            m_modelMatrixSender(*p_modelMatrix);

        for (auto mesh : p_model.GetMeshes())
        {
            Resources::Material* material = p_materials.size() > mesh->GetMaterialIndex()
                                                ? p_materials[mesh->GetMaterialIndex()]
                                                : p_defaultMaterial;
            if (material)
                DrawMesh(*mesh, *material, nullptr, nullptr);
        }
    }

    void SceneRenderer::DrawMesh(Rendering::Resources::Mesh& p_mesh,
                                 Resources::Material& p_material,
                                 FMatrix4 const* p_modelMatrix,
                                 std::vector<FMatrix4>* p_boneMatrixAry
    )
    {
        using namespace Rendering::Settings;
        p_material.Set("u_Shadowmap", m_shadowmapBuffer->GetTexture(), true);
        if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
        {
            if (p_modelMatrix)
                m_modelMatrixSender(*p_modelMatrix);

            uint8_t stateMask = p_material.GenerateStateMask();
            ApplyStateMask(stateMask);

            /* Draw the mesh */
            p_material.Bind(m_emptyTexture);
            auto shader = p_material.GetShader();
            shader->SetUniformMat4("u_LightSpaceMatrix", m_lightSpaceVPMatrix);
            shader->SetUniformVec4("u_ShadowLightPosition", m_lightInfo);
            if (m_isGPUSkin)
            {
                shader->SetUniformInt("u_IsSkinMesh", (p_mesh.isSkinMesh ? 1 : 0));
                if (p_mesh.isSkinMesh && p_boneMatrixAry != nullptr)
                {
                    shader->SetUniformMat4Array("u_BonesMatrices", *p_boneMatrixAry);
                }
            }
            Draw(p_mesh, Rendering::Settings::EPrimitiveMode::TRIANGLES,
                 p_material.GetGPUInstances());
            p_material.UnBind();
        }
    }

    void SceneRenderer::RegisterModelMatrixSender(
        std::function<void(FMatrix4)> p_modelMatrixSender)
    {
        m_modelMatrixSender = p_modelMatrixSender;
    }

    void SceneRenderer::RegisterUserMatrixSender(
        std::function<void(FMatrix4)> p_userMatrixSender)
    {
        m_userMatrixSender = p_userMatrixSender;
    }
}
