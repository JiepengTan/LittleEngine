#include "Modules/Utils/ActorUtils.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"

LittleEngine::Actor* LittleEngine::ActorUtils::CreatePlane(LittleEngine::Actor* p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Plane",p_parent,p_name);
}
LittleEngine::Actor* LittleEngine::ActorUtils::CreateCube(LittleEngine::Actor* p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Cube",p_parent,p_name);
}
LittleEngine::Actor* LittleEngine::ActorUtils::CreateSphere(LittleEngine::Actor* p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Sphere",p_parent,p_name);
}

LittleEngine::Actor* LittleEngine::ActorUtils::CreateEmptyActor( LittleEngine::Actor* p_parent, const std::string& p_name)
{
    auto& sceneManager = LittleEngine::Global::ServiceLocator::Get<SceneSystem::SceneManager>();
    const auto currentScene = sceneManager.GetCurrentScene();
    auto& instance = p_name.empty() ? currentScene->CreateActor() : currentScene->CreateActor(p_name);

    if (p_parent)
        instance.SetParent(*p_parent);
    return &instance;
}
LittleEngine::Actor* LittleEngine::ActorUtils::CreateActorWithModel(const std::string& p_modelName,
                                                             LittleEngine::Actor* p_parent, const std::string& p_name)
{
    auto path= ":Models\\" + p_modelName + ".fbx";
    auto instance = CreateEmptyActor( p_parent, p_name);

    auto& modelManager = LittleEngine::Global::ServiceLocator::Get<ResourceManagement::ModelManager>();
    auto& modelRenderer = instance->AddComponent<LittleEngine::CModelRenderer>();

    const auto model = modelManager[path];
    if (model)
        modelRenderer.SetModel(model);

    auto& materialManager = LittleEngine::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>();
    auto& materialRenderer = instance->AddComponent<LittleEngine::CMaterialRenderer>();
    const auto material = materialManager[":Materials\\Default.ovmat"];
    if (material)
        materialRenderer.FillWithMaterial(*material);
    return instance;
}
