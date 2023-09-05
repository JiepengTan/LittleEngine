#include "Modules/Utils/ActorUtils.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/ResourceManagement/MaterialManager.h"
#include "Resource/ResourceManagement/ModelManager.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"

OvCore::ECS::Actor* OvCore::ECS::ActorUtils::CreatePlane(OvCore::ECS::Actor* p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Plane",p_parent,p_name);
}
OvCore::ECS::Actor* OvCore::ECS::ActorUtils::CreateCube(OvCore::ECS::Actor* p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Cube",p_parent,p_name);
}
OvCore::ECS::Actor* OvCore::ECS::ActorUtils::CreateSphere(OvCore::ECS::Actor* p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Sphere",p_parent,p_name);
}

OvCore::ECS::Actor* OvCore::ECS::ActorUtils::CreateEmptyActor( OvCore::ECS::Actor* p_parent, const std::string& p_name)
{
    auto& sceneManager = OvCore::Global::ServiceLocator::Get<SceneSystem::SceneManager>();
    const auto currentScene = sceneManager.GetCurrentScene();
    auto& instance = p_name.empty() ? currentScene->CreateActor() : currentScene->CreateActor(p_name);

    if (p_parent)
        instance.SetParent(*p_parent);
    return &instance;
}
OvCore::ECS::Actor* OvCore::ECS::ActorUtils::CreateActorWithModel(const std::string& p_modelName,
                                                             OvCore::ECS::Actor* p_parent, const std::string& p_name)
{
    auto path= ":Models\\" + p_modelName + ".fbx";
    auto instance = CreateEmptyActor( p_parent, p_name);

    auto& modelManager = OvCore::Global::ServiceLocator::Get<ResourceManagement::ModelManager>();
    auto& modelRenderer = instance->AddComponent<OvCore::ECS::Components::CModelRenderer>();

    const auto model = modelManager[path];
    if (model)
        modelRenderer.SetModel(model);

    auto& materialManager = OvCore::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>();
    auto& materialRenderer = instance->AddComponent<OvCore::ECS::Components::CMaterialRenderer>();
    const auto material = materialManager[":Materials\\Default.ovmat"];
    if (material)
        materialRenderer.FillWithMaterial(*material);
    return instance;
}
