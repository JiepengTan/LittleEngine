/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CPointLight.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CSpotLight.h"

#include "Core/Analytics/Profiling/ProfilerSpy.h"

#include "Core/Debug/Assertion.h"

#include "../Editor/Core/EditorRenderer.h"
#include "../Editor/Core/EditorResources.h"
#include "../Editor/Panels/AView.h"
#include "../Editor/Panels/GameView.h"
#include "../Editor/Core/GizmoBehaviour.h"
#include "../Editor/Settings/EditorSettings.h"

#include "../Editor/Core/EditorActions.h"

using namespace LittleEngine;
using namespace LittleEngine::Resources;
using namespace LittleEngine::Resources;

const LittleEngine::FVector3 DEBUG_BOUNDS_COLOR = {1.0f, 0.0f, 0.0f};
const LittleEngine::FVector3 LIGHT_VOLUME_COLOR = {1.0f, 1.0f, 0.0f};
const LittleEngine::FVector3 COLLIDER_COLOR = {0.0f, 1.0f, 0.0f};
const LittleEngine::FVector3 FRUSTUM_COLOR = {1.0f, 1.0f, 1.0f};

namespace LittleEngine::Editor
{
    Editor::Core::EditorRenderer::EditorRenderer(Context& p_context) : m_context(p_context)
    {
        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::STENCIL_TEST, true);
        m_context.renderer->SetStencilOperations(LittleEngine::Rendering::Settings::EOperation::KEEP,
                                                 LittleEngine::Rendering::Settings::EOperation::KEEP,
                                                 LittleEngine::Rendering::Settings::EOperation::REPLACE);
        m_context.renderer->SetStencilAlgorithm(LittleEngine::Rendering::Settings::EComparaisonAlgorithm::ALWAYS, 1,
                                                0xFF);

        InitMaterials();

        m_context.renderer->RegisterModelMatrixSender([this](const LittleEngine::FMatrix4& p_modelMatrix)
        {
            m_context.engineUBO->SetSubData(LittleEngine::FMatrix4::Transpose(p_modelMatrix), 0);
        });

        m_context.renderer->RegisterUserMatrixSender([this](const LittleEngine::FMatrix4& p_userMatrix)
        {
            m_context.engineUBO->SetSubData
            (
                p_userMatrix,

                // UBO layout offset
                sizeof(LittleEngine::FMatrix4) +
                sizeof(LittleEngine::FMatrix4) +
                sizeof(LittleEngine::FMatrix4) +
                sizeof(LittleEngine::FVector3) +
                sizeof(float)
            );
        });
    }

    void Editor::Core::EditorRenderer::InitMaterials()
    {
        /* Default Material */
        m_defaultMaterial.SetShader(":Shaders\\Standard.glsl");
        m_defaultMaterial.SetProperty("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.f));
        m_defaultMaterial.SetProperty("u_Shininess", 100.0f);
        m_defaultMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);

        /* Empty Material */
        m_emptyMaterial.SetShader(":Shaders\\Unlit.glsl");
        m_emptyMaterial.SetProperty("u_Diffuse", FVector4(1.f, 0.f, 1.f, 1.0f));
        m_emptyMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);

        /* Grid Material */
        m_gridMaterial.SetShader(m_context.editorResources->GetShader("Grid"));
        m_gridMaterial.SetBlendable(true);
        m_gridMaterial.SetBackfaceCulling(false);
        m_gridMaterial.SetDepthTest(false);

        /* Camera Material */
        m_cameraMaterial.SetShader(":Shaders\\Lambert.glsl");
        m_cameraMaterial.SetProperty("u_Diffuse", FVector4(0.0f, 0.3f, 0.7f, 1.0f));
        m_cameraMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);

        /* Light Material */
        m_lightMaterial.SetShader(m_context.editorResources->GetShader("Billboard"));
        m_lightMaterial.SetProperty("u_Diffuse", FVector4(1.f, 1.f, 0.5f, 0.5f));
        m_lightMaterial.SetBackfaceCulling(false);
        m_lightMaterial.SetBlendable(true);
        m_lightMaterial.SetDepthTest(false);

        /* Stencil Fill Material */
        m_stencilFillMaterial.SetShader(":Shaders\\Unlit.glsl");
        m_stencilFillMaterial.SetBackfaceCulling(true);
        m_stencilFillMaterial.SetDepthTest(false);
        m_stencilFillMaterial.SetColorWriting(false);
        m_stencilFillMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);

        /* Texture Material */
        m_textureMaterial.SetShader(":Shaders\\Unlit.glsl");
        m_textureMaterial.SetProperty("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.f));
        m_textureMaterial.SetBackfaceCulling(false);
        m_textureMaterial.SetBlendable(true);
        m_textureMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);

        /* Outline Material */
        m_outlineMaterial.SetShader(":Shaders\\Unlit.glsl");
        m_outlineMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);
        m_outlineMaterial.SetDepthTest(false);

        /* Gizmo Arrow Material */
        m_gizmoArrowMaterial.SetShader(m_context.editorResources->GetShader("Gizmo"));
        m_gizmoArrowMaterial.SetGPUInstances(3);
        m_gizmoArrowMaterial.SetProperty("u_IsBall", false);
        m_gizmoArrowMaterial.SetProperty("u_IsPickable", false);

        /* Gizmo Ball Material */
        m_gizmoBallMaterial.SetShader(m_context.editorResources->GetShader("Gizmo"));
        m_gizmoBallMaterial.SetProperty("u_IsBall", true);
        m_gizmoBallMaterial.SetProperty("u_IsPickable", false);

        /* Gizmo Pickable Material */
        m_gizmoPickingMaterial.SetShader(m_context.editorResources->GetShader("Gizmo"));
        m_gizmoPickingMaterial.SetGPUInstances(3);
        m_gizmoPickingMaterial.SetProperty("u_IsBall", false);
        m_gizmoPickingMaterial.SetProperty("u_IsPickable", true);

        /* Picking Material */
        m_actorPickingMaterial.SetShader(":Shaders\\Unlit.glsl");
        m_actorPickingMaterial.SetProperty("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.0f));
        m_actorPickingMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);
        m_actorPickingMaterial.SetFrontfaceCulling(false);
        m_actorPickingMaterial.SetBackfaceCulling(false);
    }

    void Editor::Core::EditorRenderer::PreparePickingMaterial(
        ActorPtr p_actor, LittleEngine::Material& p_material)
    {
        uint32_t actorID = static_cast<uint32_t>(p_actor->GetID());

        auto bytes = reinterpret_cast<uint8_t*>(&actorID);
        auto color = FVector4{bytes[0] / 255.0f, bytes[1] / 255.0f, bytes[2] / 255.0f, 1.0f};

        p_material.SetProperty("u_Diffuse", color);
    }

    LittleEngine::FMatrix4 Editor::Core::EditorRenderer::CalculateCameraModelMatrix(ActorPtr p_actor)
    {
        auto translation = FMatrix4::Translation(p_actor->transform->GetWorldPosition());
        auto rotation = FQuaternion::ToMatrix4(p_actor->transform->GetWorldRotation());

        return translation * rotation;
    }

    void Editor::Core::EditorRenderer::RenderScene(const LittleEngine::FVector3& p_cameraPosition,
                                                   const LittleEngine::Rendering::LowRenderer::Camera&
                                                   p_camera,
                                                   const LittleEngine::Rendering::Data::Frustum*
                                                   p_customFrustum)
    {
        /* Render the actors */
        m_context.lightSSBO->Bind(0);
        m_context.renderer->RenderScene(*m_context.sceneManager.GetCurrentScene(), p_cameraPosition, p_camera,
                                        p_customFrustum, &m_emptyMaterial);
        m_context.lightSSBO->Unbind();
    }

    void Editor::Core::EditorRenderer::RenderSceneForActorPicking()
    {
        auto& scene = *m_context.sceneManager.GetCurrentScene();

        /* Render models */
        for (auto renderID : scene.GetFastAccessComponents().modelRenderers)
        {
            auto modelRenderer = scene.GetActor(renderID)->GetComponent<CModelRenderer>();
            auto actor = modelRenderer->GetActor();

            if (actor->IsActive())
            {
                if (auto model = modelRenderer->GetModel())
                {
                    if (auto materialRenderer = modelRenderer->GetActor()->GetComponent<
                        LittleEngine::CMaterialRenderer>())
                    {
                        const auto& modelMatrix = actor->transform->GetWorldMatrix();

                        PreparePickingMaterial(actor, m_actorPickingMaterial);

                        for (auto mesh : model->GetMeshes())
                        {
                            LittleEngine::Material* material = nullptr;

                            if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
                            {
                                material = materialRenderer->GetMaterial(mesh->GetMaterialIndex());
                                if (!material || !material->GetShader())
                                    material = &m_emptyMaterial;
                            }

                            if (material)
                            {
                                m_actorPickingMaterial.SetBackfaceCulling(material->HasBackfaceCulling());
                                m_actorPickingMaterial.SetFrontfaceCulling(material->HasFrontfaceCulling());
                                m_actorPickingMaterial.SetColorWriting(material->HasColorWriting());
                                m_actorPickingMaterial.SetDepthTest(material->HasDepthTest());
                                m_actorPickingMaterial.SetDepthWriting(material->HasDepthWriting());

                                m_context.renderer->DrawMesh(*mesh, m_actorPickingMaterial, &modelMatrix);
                            }
                        }
                    }
                }
            }
        }

        /* Render cameras */
        for (auto cameraId : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().cameras)
        {
            auto actor =scene.GetActor(cameraId);

            if (actor->IsActive())
            {
                PreparePickingMaterial(actor, m_actorPickingMaterial);
                auto& model = *m_context.editorResources->GetModel("Camera");
                auto modelMatrix = CalculateCameraModelMatrix(actor);

                m_context.renderer->DrawModelWithSingleMaterial(model, m_actorPickingMaterial, &modelMatrix);
            }
        }

        /* Render lights */
        if (Settings::EditorSettings::LightBillboardScale > 0.001f)
        {
            m_context.renderer->Clear(false, true, false);

            m_lightMaterial.SetDepthTest(true);
            m_lightMaterial.SetProperty<float>("u_Scale", Settings::EditorSettings::LightBillboardScale * 0.1f);
            m_lightMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", nullptr);

            for (auto lightId : m_context.sceneManager.GetCurrentScene()->GetFastAccessComponents().lights)
            {
                auto actor = scene.GetActor(lightId);
                if (actor->IsActive())
                    PreparePickingMaterial(actor, m_lightMaterial);
                auto& model = *m_context.editorResources->GetModel("Vertical_Plane");
                auto modelMatrix = FMatrix4::Translation(actor->transform->GetWorldPosition());
                m_context.renderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
            }
        }
    }


    void LittleEngine::Editor::Core::EditorRenderer::RenderUI()
    {
        m_context.uiManager->Render();
    }

    void LittleEngine::Editor::Core::EditorRenderer::RenderCameras()
    {
        using namespace LittleEngine;
        auto curScene = m_context.sceneManager.GetCurrentScene();

        for (auto cameraId : curScene->GetFastAccessComponents().cameras)
        {
            auto actor = curScene->GetActor(cameraId);
            if (actor!= nullptr&& actor->IsActive())
            {
                auto& model = *m_context.editorResources->GetModel("Camera");
                auto modelMatrix = CalculateCameraModelMatrix(actor);

                m_context.renderer->DrawModelWithSingleMaterial(model, m_cameraMaterial, &modelMatrix);
            }
        }
    }

    void LittleEngine::Editor::Core::EditorRenderer::RenderLights()
    {
        using namespace LittleEngine;

        m_lightMaterial.SetDepthTest(false);
        m_lightMaterial.SetProperty<float>("u_Scale", Settings::EditorSettings::LightBillboardScale * 0.1f);
        auto scene = m_context.sceneManager.GetCurrentScene();
        for (auto lightId : scene->GetFastAccessComponents().lights)
        {
            auto actor = scene->GetActor(lightId);
            if(actor == nullptr) continue;
            auto light = actor->GetComponent<CLight>();
            if(light == nullptr) continue;
            if (actor->IsActive())
            {
                auto& model = *m_context.editorResources->GetModel("Vertical_Plane");
                auto modelMatrix = FMatrix4::Translation(actor->transform->GetWorldPosition());

                LittleEngine::Texture* texture = nullptr;
                auto lightType = static_cast<LittleEngine::Rendering::Entities::Light::Type>(
                                            static_cast<int>(light->GetData().type));
                switch (lightType)
                {
                case LittleEngine::Rendering::Entities::Light::Type::POINT:
                    texture = m_context.editorResources-> GetTexture("Bill_Point_Light");break;
                case LittleEngine::Rendering::Entities::Light::Type::SPOT:
                    texture = m_context.editorResources->GetTexture("Bill_Spot_Light"); break;
                case LittleEngine::Rendering::Entities::Light::Type::DIRECTIONAL:
                    texture = m_context.editorResources-> GetTexture("Bill_Directional_Light");    break;
                case LittleEngine::Rendering::Entities::Light::Type::AMBIENT_BOX:
                    texture = m_context.editorResources->GetTexture("Bill_Ambient_Box_Light");   break;
                case LittleEngine::Rendering::Entities::Light::Type::AMBIENT_SPHERE:
                    texture = m_context.editorResources ->GetTexture("Bill_Ambient_Sphere_Light");   break;
                }

                const auto& lightColor = light->GetColor();
                m_lightMaterial.SetProperty<LittleEngine::Texture*>("u_DiffuseMap", texture);
                m_lightMaterial.SetProperty<LittleEngine::FVector4>("u_Diffuse",
                                                            LittleEngine::FVector4(
                                                                lightColor.x, lightColor.y, lightColor.z, 0.75f));
                m_context.renderer->DrawModelWithSingleMaterial(model, m_lightMaterial, &modelMatrix);
            }
        }
    }

    void Editor::Core::EditorRenderer::RenderGizmo(const LittleEngine::FVector3& p_position,
                                                   const LittleEngine::FQuaternion& p_rotation,
                                                   Core::EGizmoOperation
                                                   p_operation, bool p_pickable, int p_highlightedAxis)
    {
        using namespace LittleEngine;

        FMatrix4 model = FMatrix4::Translation(p_position) * FQuaternion::ToMatrix4(FQuaternion::Normalize(p_rotation));

        LittleEngine::Model* arrowModel = nullptr;

        if (!p_pickable)
        {
            FMatrix4 sphereModel = model * LittleEngine::FMatrix4::Scaling({0.1f, 0.1f, 0.1f});
            m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Sphere"),
                                                            m_gizmoBallMaterial, &sphereModel);
            m_gizmoArrowMaterial.SetProperty("u_HighlightedAxis", p_highlightedAxis);

            switch (p_operation)
            {
            case Core::EGizmoOperation::TRANSLATE:
                arrowModel = m_context.editorResources->GetModel("Arrow_Translate");
                break;
            case Core::EGizmoOperation::ROTATE:
                arrowModel = m_context.editorResources->GetModel("Arrow_Rotate");
                break;
            case Core::EGizmoOperation::SCALE:
                arrowModel = m_context.editorResources->GetModel("Arrow_Scale");
                break;
            }
        }
        else
        {
            arrowModel = m_context.editorResources->GetModel("Arrow_Picking");
        }

        if (arrowModel)
        {
            m_context.renderer->DrawModelWithSingleMaterial(*arrowModel,
                                                            p_pickable ? m_gizmoPickingMaterial : m_gizmoArrowMaterial,
                                                            &model);
        }
    }

    void Editor::Core::EditorRenderer::RenderModelToStencil(
        const LittleEngine::FMatrix4& p_worldMatrix, LittleEngine::Model& p_model)
    {
        m_context.renderer->SetStencilMask(0xFF);
        m_context.renderer->DrawModelWithSingleMaterial(p_model, m_stencilFillMaterial, &p_worldMatrix);
        m_context.renderer->SetStencilMask(0x00);
    }

    void Editor::Core::EditorRenderer::RenderModelOutline(const LittleEngine::FMatrix4& p_worldMatrix,
                                                          LittleEngine::Model&
                                                          p_model, float p_width)
    {
        m_context.renderer->SetStencilAlgorithm(LittleEngine::Rendering::Settings::EComparaisonAlgorithm::NOTEQUAL, 1,
                                                0xFF);
        m_context.renderer->SetRasterizationMode(LittleEngine::Rendering::Settings::ERasterizationMode::LINE);
        m_context.renderer->SetRasterizationLinesWidth(p_width);
        m_context.renderer->DrawModelWithSingleMaterial(p_model, m_outlineMaterial, &p_worldMatrix);
        m_context.renderer->SetRasterizationLinesWidth(1.f);
        m_context.renderer->SetRasterizationMode(LittleEngine::Rendering::Settings::ERasterizationMode::FILL);
        m_context.renderer->SetStencilAlgorithm(LittleEngine::Rendering::Settings::EComparaisonAlgorithm::ALWAYS, 1,
                                                0xFF);
    }

    void Editor::Core::EditorRenderer::RenderActorOutlinePass(
        ActorPtr p_actor, bool p_toStencil, bool p_isSelected)
    {
        float outlineWidth = p_isSelected ? 5.0f : 2.5f;

        m_outlineMaterial.SetProperty("u_Diffuse",
                              p_isSelected ? FVector4(1.f, 0.7f, 0.f, 1.0f) : FVector4(1.f, 1.f, 0.f, 1.0f));

        if (p_actor->IsActive())
        {
            /* Render static mesh outline and bounding spheres */
            if (auto modelRenderer = p_actor->GetComponent<LittleEngine::CModelRenderer>(); modelRenderer &&
                modelRenderer->GetModel())
            {
                if (p_toStencil)
                    RenderModelToStencil(p_actor->transform->GetWorldMatrix(), *modelRenderer->GetModel());
                else
                    RenderModelOutline(p_actor->transform->GetWorldMatrix(), *modelRenderer->GetModel(), outlineWidth);

                if (p_isSelected && Settings::EditorSettings::ShowGeometryBounds && !p_toStencil)
                {
                    RenderBoundingSpheres(*modelRenderer);
                }
            }

            /* Render camera component outline */
            if (auto cameraComponent = p_actor->GetComponent<LittleEngine::CCamera>(); cameraComponent)
            {
                auto model = CalculateCameraModelMatrix(p_actor);

                if (p_toStencil)
                    RenderModelToStencil(model, *m_context.editorResources->GetModel("Camera"));
                else
                    RenderModelOutline(model, *m_context.editorResources->GetModel("Camera"), outlineWidth);

                if (p_isSelected && !p_toStencil)
                {
                    RenderCameraFrustum(*cameraComponent);
                }
            }

            if (p_isSelected && !p_toStencil)
            {
                /* Render the actor collider */
                if (p_actor->GetComponent<LittleEngine::CPhysicalObject>())
                {
                    RenderActorCollider(p_actor);
                }

                /* Render the actor ambient light */
                if (auto ambientBoxComp = p_actor->GetComponent<LittleEngine::CAmbientBoxLight>())
                {
                    RenderAmbientBoxVolume(*ambientBoxComp);
                }

                if (auto ambientSphereComp = p_actor->GetComponent<LittleEngine::CAmbientSphereLight>())
                {
                    RenderAmbientSphereVolume(*ambientSphereComp);
                }

                if (Settings::EditorSettings::ShowLightBounds)
                {
                    if (auto light = p_actor->GetComponent<LittleEngine::CLight>())
                    {
                        RenderLightBounds(*light);
                    }
                }
            }

            for (auto& child : p_actor->GetChildren())
            {
                RenderActorOutlinePass(child, p_toStencil, p_isSelected);
            }
        }
    }

    void DrawFrustumLines(LittleEngine::Rendering::Core::ShapeDrawer& p_drawer,
                          const LittleEngine::FVector3& pos,
                          const LittleEngine::FVector3& forward,
                          float near,
                          const float far,
                          const LittleEngine::FVector3& a,
                          const LittleEngine::FVector3& b,
                          const LittleEngine::FVector3& c,
                          const LittleEngine::FVector3& d,
                          const LittleEngine::FVector3& e,
                          const LittleEngine::FVector3& f,
                          const LittleEngine::FVector3& g,
                          const LittleEngine::FVector3& h)
    {
        // Convenient lambda to draw a frustum line
        auto draw = [&](const FVector3& p_start, const FVector3& p_end, const float planeDistance)
        {
            auto offset = pos + forward * planeDistance;
            auto start = offset + p_start;
            auto end = offset + p_end;
            p_drawer.DrawLine(start, end, FRUSTUM_COLOR);
        };

        // Draw near plane
        draw(a, b, near);
        draw(b, d, near);
        draw(d, c, near);
        draw(c, a, near);

        // Draw far plane
        draw(e, f, far);
        draw(f, h, far);
        draw(h, g, far);
        draw(g, e, far);

        // Draw lines between near and far planes
        draw(a + forward * near, e + forward * far, 0);
        draw(b + forward * near, f + forward * far, 0);
        draw(c + forward * near, g + forward * far, 0);
        draw(d + forward * near, h + forward * far, 0);
    }

    void Editor::Core::EditorRenderer::RenderCameraPerspectiveFrustum(
        std::pair<uint16_t, uint16_t>& p_size, LittleEngine::CCamera& p_camera)
    {
        const auto actor = p_camera.GetActor();
        auto& camera = p_camera.GetCamera();

        const auto& cameraPos = actor->transform->GetWorldPosition();
        const auto& cameraRotation = actor->transform->GetWorldRotation();
        const auto& cameraForward = actor->transform->GetWorldForward();

        camera.CacheMatrices(p_size.first, p_size.second, cameraPos, cameraRotation);
        const auto proj = FMatrix4::Transpose(camera.GetProjectionMatrix());
        const auto near = camera.GetNear();
        const auto far = camera.GetFar();

        const auto nLeft = near * (proj.data[2] - 1.0f) / proj.data[0];
        const auto nRight = near * (1.0f + proj.data[2]) / proj.data[0];
        const auto nTop = near * (1.0f + proj.data[6]) / proj.data[5];
        const auto nBottom = near * (proj.data[6] - 1.0f) / proj.data[5];

        const auto fLeft = far * (proj.data[2] - 1.0f) / proj.data[0];
        const auto fRight = far * (1.0f + proj.data[2]) / proj.data[0];
        const auto fTop = far * (1.0f + proj.data[6]) / proj.data[5];
        const auto fBottom = far * (proj.data[6] - 1.0f) / proj.data[5];

        auto a = cameraRotation * FVector3{nLeft, nTop, 0};
        auto b = cameraRotation * FVector3{nRight, nTop, 0};
        auto c = cameraRotation * FVector3{nLeft, nBottom, 0};
        auto d = cameraRotation * FVector3{nRight, nBottom, 0};
        auto e = cameraRotation * FVector3{fLeft, fTop, 0};
        auto f = cameraRotation * FVector3{fRight, fTop, 0};
        auto g = cameraRotation * FVector3{fLeft, fBottom, 0};
        auto h = cameraRotation * FVector3{fRight, fBottom, 0};

        DrawFrustumLines(*m_context.shapeDrawer, cameraPos, cameraForward, near, far, a, b, c, d, e, f, g, h);
    }

    void Editor::Core::EditorRenderer::RenderCameraOrthographicFrustum(
        std::pair<uint16_t, uint16_t>& p_size, LittleEngine::CCamera& p_camera)
    {
        auto owner = p_camera.GetActor();
        auto& camera = p_camera.GetCamera();
        const auto ratio = p_size.first / static_cast<float>(p_size.second);

        const auto& cameraPos = owner->transform->GetWorldPosition();
        const auto& cameraRotation = owner->transform->GetWorldRotation();
        const auto& cameraForward = owner->transform->GetWorldForward();

        const auto near = camera.GetNear();
        const auto far = camera.GetFar();
        const auto size = p_camera.GetSize();

        const auto right = ratio * size;
        const auto left = -right;
        const auto top = size;
        const auto bottom = -top;

        const auto a = cameraRotation * FVector3{left, top, 0};
        const auto b = cameraRotation * FVector3{right, top, 0};
        const auto c = cameraRotation * FVector3{left, bottom, 0};
        const auto d = cameraRotation * FVector3{right, bottom, 0};

        DrawFrustumLines(*m_context.shapeDrawer, cameraPos, cameraForward, near, far, a, b, c, d, a, b, c, d);
    }

    void Editor::Core::EditorRenderer::RenderCameraFrustum(LittleEngine::CCamera& p_camera)
    {
        auto& gameView = EDITOR_PANEL(Panels::GameView, "Game View");
        auto gameViewSize = gameView.GetSafeSize();

        if (gameViewSize.first == 0 || gameViewSize.second == 0)
        {
            gameViewSize = {16, 9};
        }

        switch (p_camera.GetProjectionMode())
        {
        case LittleEngine::Rendering::Settings::EProjectionMode::ORTHOGRAPHIC:
            RenderCameraOrthographicFrustum(gameViewSize, p_camera);
            break;

        case LittleEngine::Rendering::Settings::EProjectionMode::PERSPECTIVE:
            RenderCameraPerspectiveFrustum(gameViewSize, p_camera);
            break;
        }
    }

    void Editor::Core::EditorRenderer::RenderActorCollider(ActorPtr p_actor)
    {
        using namespace LittleEngine;
        using namespace LittleEngine::Physics::Entities;

        bool depthTestBackup = m_context.renderer->GetCapability(
            LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST);
        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

        /* Draw the box collider if any */
        if (auto boxColliderComponent = p_actor->GetComponent<LittleEngine::CPhysicalBox>(); boxColliderComponent)
        {
            LittleEngine::FQuaternion rotation = p_actor->transform->GetWorldRotation();
            LittleEngine::FVector3 center = p_actor->transform->GetWorldPosition();
            LittleEngine::FVector3 colliderSize = boxColliderComponent->GetSize();
            LittleEngine::FVector3 actorScale = p_actor->transform->GetWorldScale();
            LittleEngine::FVector3 halfSize = {
                colliderSize.x * actorScale.x, colliderSize.y * actorScale.y, colliderSize.z * actorScale.z
            };
            LittleEngine::FVector3 size = halfSize * 2.f;

            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, -halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{-halfSize.x, -halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{-halfSize.x, +halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, -halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{-halfSize.x, +halfSize.y, -halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, -halfSize.y, +halfSize.z},
                center + rotation * LittleEngine::FVector3{-halfSize.x, +halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{+halfSize.x, -halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, -halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{+halfSize.x, halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, +halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{+halfSize.x, -halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, +halfSize.y, -halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{+halfSize.x, -halfSize.y, +halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, +halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, -halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, -halfSize.y, -halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, +halfSize.y, -halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, +halfSize.y, -halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, -halfSize.y, +halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, -halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * LittleEngine::FVector3{-halfSize.x, +halfSize.y, +halfSize.z},
                center + rotation * LittleEngine::FVector3{+halfSize.x, +halfSize.y, +halfSize.z},
                LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
        }

        /* Draw the sphere collider if any */
        if (auto sphereColliderComponent = p_actor->GetComponent<LittleEngine::CPhysicalSphere>();
            sphereColliderComponent)
        {
            FVector3 actorScale = p_actor->transform->GetWorldScale();
            LittleEngine::FQuaternion rotation = p_actor->transform->GetWorldRotation();
            LittleEngine::FVector3 center = p_actor->transform->GetWorldPosition();
            float radius = sphereColliderComponent->GetRadius() * std::max(
                std::max(std::max(actorScale.x, actorScale.y), actorScale.z), 0.0f);

            for (float i = 0; i <= 360.0f; i += 10.0f)
            {
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (LittleEngine::FVector3
                        {cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f} * radius),
                    center + rotation * (LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                    } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (LittleEngine::FVector3
                        {0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))} * radius),
                    center + rotation * (LittleEngine::FVector3{
                        0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                    } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (LittleEngine::FVector3
                        {cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))} * radius),
                    center + rotation * (LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                    } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            }
        }

        /* Draw the capsule collider if any */
        if (auto capsuleColliderComponent = p_actor->GetComponent<LittleEngine::CPhysicalCapsule>();
            capsuleColliderComponent)
        {
            float radius = abs(capsuleColliderComponent->GetRadius() * std::max(
                std::max(p_actor->transform->GetWorldScale().x, p_actor->transform->GetWorldScale().z), 0.f));
            float height = abs(capsuleColliderComponent->GetHeight() * p_actor->transform->GetWorldScale().y);
            float halfHeight = height / 2;

            FVector3 actorScale = p_actor->transform->GetWorldScale();
            LittleEngine::FQuaternion rotation = p_actor->transform->GetWorldRotation();
            LittleEngine::FVector3 center = p_actor->transform->GetWorldPosition();

            LittleEngine::FVector3 hVec = {0.0f, halfHeight, 0.0f};
            for (float i = 0; i < 360.0f; i += 10.0f)
            {
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (hVec + LittleEngine::FVector3
                        {cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))} * radius),
                    center + rotation * (hVec + LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                    } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (-hVec + LittleEngine::FVector3
                        {cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))} * radius),
                    center + rotation * (-hVec + LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                    } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);

                if (i < 180.f)
                {
                    m_context.shapeDrawer->DrawLine(
                        center + rotation * (hVec + LittleEngine::FVector3{
                            cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f
                        } * radius), center + rotation * (hVec + LittleEngine::FVector3{
                            cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                        } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                    m_context.shapeDrawer->DrawLine(
                        center + rotation * (hVec + LittleEngine::FVector3{
                            0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))
                        } * radius), center + rotation * (hVec + LittleEngine::FVector3{
                            0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                        } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                }
                else
                {
                    m_context.shapeDrawer->DrawLine(
                        center + rotation * (-hVec + LittleEngine::FVector3{
                            cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f
                        } * radius), center + rotation * (-hVec + LittleEngine::FVector3{
                            cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                        } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                    m_context.shapeDrawer->DrawLine(
                        center + rotation * (-hVec + LittleEngine::FVector3{
                            0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))
                        } * radius), center + rotation * (-hVec + LittleEngine::FVector3{
                            0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                        } * radius), LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
                }
            }

            m_context.shapeDrawer->DrawLine(center + rotation * (LittleEngine::FVector3{-radius, -halfHeight, 0.f}),
                                            center + rotation * (LittleEngine::FVector3{-radius, +halfHeight, 0.f}),
                                            LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(center + rotation * (LittleEngine::FVector3{radius, -halfHeight, 0.f}),
                                            center + rotation * (LittleEngine::FVector3{radius, +halfHeight, 0.f}),
                                            LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(center + rotation * (LittleEngine::FVector3{0.f, -halfHeight, -radius}),
                                            center + rotation * (LittleEngine::FVector3{0.f, +halfHeight, -radius}),
                                            LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
            m_context.shapeDrawer->DrawLine(center + rotation * (LittleEngine::FVector3{0.f, -halfHeight, radius}),
                                            center + rotation * (LittleEngine::FVector3{0.f, +halfHeight, radius}),
                                            LittleEngine::FVector3{0.f, 1.f, 0.f}, 1.f);
        }

        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST,
                                          depthTestBackup);
        m_context.renderer->SetRasterizationLinesWidth(1.0f);
    }

    void Editor::Core::EditorRenderer::RenderLightBounds(LittleEngine::CLight& p_light)
    {
        bool depthTestBackup = m_context.renderer->GetCapability(
            LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST);
        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

        auto& data = p_light.GetData();

        LittleEngine::FQuaternion rotation = data.GetTransform()->GetWorldRotation();
        LittleEngine::FVector3 center = data.GetTransform()->GetWorldPosition();
        float radius = data.GetEffectRange();

        if (!std::isinf(radius))
        {
            for (float i = 0; i <= 360.0f; i += 10.0f)
            {
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (LittleEngine::FVector3{
                        cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f
                    } * radius), center + rotation * (LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                    } * radius), DEBUG_BOUNDS_COLOR, 1.f);
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (LittleEngine::FVector3{
                        0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))
                    } * radius), center + rotation * (LittleEngine::FVector3{
                        0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                    } * radius), DEBUG_BOUNDS_COLOR, 1.f);
                m_context.shapeDrawer->DrawLine(
                    center + rotation * (LittleEngine::FVector3{
                        cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))
                    } * radius), center + rotation * (LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                    } * radius), DEBUG_BOUNDS_COLOR, 1.f);
            }
        }

        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST,
                                          depthTestBackup);
    }

    void Editor::Core::EditorRenderer::RenderAmbientBoxVolume(
        LittleEngine::CAmbientBoxLight& p_ambientBoxLight)
    {
        bool depthTestBackup = m_context.renderer->GetCapability(
            LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST);
        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

        auto& data = p_ambientBoxLight.GetData();

        FMatrix4 model =
            FMatrix4::Translation(p_ambientBoxLight.GetActor()->transform->GetWorldPosition()) *
            FMatrix4::Scaling({data.constant * 2.f, data.linear * 2.f, data.quadratic * 2.f});

        LittleEngine::FVector3 center = p_ambientBoxLight.GetActor()->transform->GetWorldPosition();
        LittleEngine::FVector3 size = {data.constant * 2.f, data.linear * 2.f, data.quadratic * 2.f};
        LittleEngine::FVector3 actorScale = p_ambientBoxLight.GetActor()->transform->GetWorldScale();
        LittleEngine::FVector3 halfSize = size / 2.f;

        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, -halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{-halfSize.x, -halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{-halfSize.x, +halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, -halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{-halfSize.x, +halfSize.y, -halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, -halfSize.y, +halfSize.z},
                                        center + LittleEngine::FVector3{-halfSize.x, +halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{+halfSize.x, -halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, -halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{+halfSize.x, halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, +halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{+halfSize.x, -halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, +halfSize.y, -halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{+halfSize.x, -halfSize.y, +halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, +halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, -halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, -halfSize.y, -halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, +halfSize.y, -halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, +halfSize.y, -halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, -halfSize.y, +halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, -halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);
        m_context.shapeDrawer->DrawLine(center + LittleEngine::FVector3{-halfSize.x, +halfSize.y, +halfSize.z},
                                        center + LittleEngine::FVector3{+halfSize.x, +halfSize.y, +halfSize.z},
                                        LIGHT_VOLUME_COLOR, 1.f);

        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST,
                                          depthTestBackup);
    }

    void Editor::Core::EditorRenderer::RenderAmbientSphereVolume(
        LittleEngine::CAmbientSphereLight& p_ambientSphereLight)
    {
        bool depthTestBackup = m_context.renderer->GetCapability(
            LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST);
        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

        auto& data = p_ambientSphereLight.GetData();

        LittleEngine::FQuaternion rotation = p_ambientSphereLight.GetActor()->transform->GetWorldRotation();
        LittleEngine::FVector3 center = p_ambientSphereLight.GetActor()->transform->GetWorldPosition();
        float radius = data.constant;

        for (float i = 0; i <= 360.0f; i += 10.0f)
        {
            m_context.shapeDrawer->DrawLine(
                center + rotation * (LittleEngine::FVector3{cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f} *
                    radius), center + rotation * (LittleEngine::FVector3{
                    cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                } * radius), LIGHT_VOLUME_COLOR, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * (LittleEngine::FVector3{0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))} *
                    radius), center + rotation * (LittleEngine::FVector3{
                    0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                } * radius), LIGHT_VOLUME_COLOR, 1.f);
            m_context.shapeDrawer->DrawLine(
                center + rotation * (LittleEngine::FVector3{cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))} *
                    radius), center + rotation * (LittleEngine::FVector3{
                    cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                } * radius), LIGHT_VOLUME_COLOR, 1.f);
        }

        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST,
                                          depthTestBackup);
    }

    void Editor::Core::EditorRenderer::RenderBoundingSpheres(
        LittleEngine::CModelRenderer& p_modelRenderer)
    {
        using namespace LittleEngine;
        using namespace LittleEngine::Physics::Entities;

        bool depthTestBackup = m_context.renderer->GetCapability(
            LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST);
        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST, false);

        /* Draw the sphere collider if any */
        if (auto model = p_modelRenderer.GetModel())
        {
            auto actor = p_modelRenderer.GetActor();

            LittleEngine::FVector3 actorScale = actor->transform->GetWorldScale();
            LittleEngine::FQuaternion actorRotation = actor->transform->GetWorldRotation();
            LittleEngine::FVector3 actorPosition = actor->transform->GetWorldPosition();

            const auto& modelBoundingsphere =
                p_modelRenderer.GetFrustumBehaviour() == LittleEngine::CModelRenderer::EFrustumBehaviour::CULL_CUSTOM
                    ? p_modelRenderer.GetCustomBoundingSphere()
                    : model->GetBoundingSphere();

            float radiusScale = std::max(std::max(std::max(actorScale.x, actorScale.y), actorScale.z), 0.0f);
            float scaledRadius = modelBoundingsphere.radius * radiusScale;
            auto sphereOffset = LittleEngine::FQuaternion::RotatePoint(modelBoundingsphere.position, actorRotation) *
                radiusScale;

            LittleEngine::FVector3 boundingSphereCenter = actorPosition + sphereOffset;

            for (float i = 0; i <= 360.0f; i += 10.0f)
            {
                m_context.shapeDrawer->DrawLine(
                    boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                        cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f
                    } * scaledRadius), boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                    } * scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
                m_context.shapeDrawer->DrawLine(
                    boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                        0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))
                    } * scaledRadius), boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                        0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                    } * scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
                m_context.shapeDrawer->DrawLine(
                    boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                        cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))
                    } * scaledRadius), boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                        cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                    } * scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
            }

            if (p_modelRenderer.GetFrustumBehaviour() == LittleEngine::CModelRenderer::EFrustumBehaviour::CULL_MESHES)
            {
                const auto& meshes = model->GetMeshes();

                if (meshes.size() > 1) // One mesh would result into the same bounding sphere for mesh and model
                {
                    for (auto mesh : meshes)
                    {
                        auto& meshBoundingSphere = mesh->GetBoundingSphere();
                        float scaledRadius = meshBoundingSphere.radius * radiusScale;
                        auto sphereOffset = LittleEngine::FQuaternion::RotatePoint(
                            meshBoundingSphere.position, actorRotation) * radiusScale;

                        LittleEngine::FVector3 boundingSphereCenter = actorPosition + sphereOffset;

                        for (float i = 0; i <= 360.0f; i += 10.0f)
                        {
                            m_context.shapeDrawer->DrawLine(
                                boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                                    cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f
                                } * scaledRadius), boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                                    cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f
                                } * scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
                            m_context.shapeDrawer->DrawLine(
                                boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                                    0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f))
                                } * scaledRadius), boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                                    0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f))
                                } * scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
                            m_context.shapeDrawer->DrawLine(
                                boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                                    cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f))
                                } * scaledRadius), boundingSphereCenter + actorRotation * (LittleEngine::FVector3{
                                    cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f))
                                } * scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
                        }
                    }
                }
            }
        }

        m_context.renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::DEPTH_TEST,
                                          depthTestBackup);
        m_context.renderer->SetRasterizationLinesWidth(1.0f);
    }

    void Editor::Core::EditorRenderer::RenderModelAsset(
        LittleEngine::Model& p_model)
    {
        FMatrix4 model = LittleEngine::FMatrix4::Scaling({3.f, 3.f, 3.f});
        m_context.renderer->DrawModelWithSingleMaterial(p_model, m_defaultMaterial, &model);
    }

    void Editor::Core::EditorRenderer::RenderTextureAsset(
        LittleEngine::Texture& p_texture)
    {
        FMatrix4 model = FMatrix4::RotateOnAxisX(FMatrix4::Scaling({5.f, 5.f, 5.f}), 90.f * 0.0174f);

        m_textureMaterial.SetProperty<Texture*>("u_DiffuseMap", &p_texture);
        m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Plane"),
                                                        m_textureMaterial, &model);
    }

    void Editor::Core::EditorRenderer::RenderMaterialAsset(LittleEngine::Material& p_material)
    {
        FMatrix4 model = LittleEngine::FMatrix4::Scaling({3.f, 3.f, 3.f});
        m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Sphere"), p_material,
                                                        &model, &m_emptyMaterial);
    }

    void Editor::Core::EditorRenderer::RenderGrid(const LittleEngine::FVector3& p_viewPos,
                                                  const LittleEngine::FVector3& p_color)
    {
        constexpr float gridSize = 5000.0f;

        FMatrix4 model = FMatrix4::Translation({p_viewPos.x, 0.0f, p_viewPos.z}) * FMatrix4::Scaling({
            gridSize * 2.0f, 1.f, gridSize * 2.0f
        });
        m_gridMaterial.SetProperty("u_Color", p_color);
        m_context.renderer->DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Plane"), m_gridMaterial,
                                                        &model);

        m_context.shapeDrawer->DrawLine(LittleEngine::FVector3(-gridSize + p_viewPos.x, 0.0f, 0.0f),
                                        LittleEngine::FVector3(gridSize + p_viewPos.x, 0.0f, 0.0f),
                                        LittleEngine::FVector3(1.0f, 0.0f, 0.0f), 1.0f);
        m_context.shapeDrawer->DrawLine(LittleEngine::FVector3(0.0f, -gridSize + p_viewPos.y, 0.0f),
                                        LittleEngine::FVector3(0.0f, gridSize + p_viewPos.y, 0.0f),
                                        LittleEngine::FVector3(0.0f, 1.0f, 0.0f), 1.0f);
        m_context.shapeDrawer->DrawLine(LittleEngine::FVector3(0.0f, 0.0f, -gridSize + p_viewPos.z),
                                        LittleEngine::FVector3(0.0f, 0.0f, gridSize + p_viewPos.z),
                                        LittleEngine::FVector3(0.0f, 0.0f, 1.0f), 1.0f);
    }

    void Editor::Core::EditorRenderer::UpdateLights(LittleEngine::Scene& p_scene)
    {
        PROFILER_SPY("Light SSBO Update");
        auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
        m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
    }

    void Editor::Core::EditorRenderer::UpdateLightsInFrustum(
        LittleEngine::Scene& p_scene, const LittleEngine::Rendering::Data::Frustum& p_frustum)
    {
        PROFILER_SPY("Light SSBO Update (Frustum culled)");
        auto lightMatrices = m_context.renderer->FindLightMatricesInFrustum(p_scene, p_frustum);
        m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
    }
}
