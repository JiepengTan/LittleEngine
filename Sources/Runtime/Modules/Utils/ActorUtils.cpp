#include "Modules/Utils/ActorUtils.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/MaterialManager.h"
#include "Resource/Manager/ModelManager.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"

LittleEngine::ActorPtr LittleEngine::ActorUtils::CreatePlane(LittleEngine::ActorPtr p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Plane",p_parent,p_name);
}
LittleEngine::ActorPtr LittleEngine::ActorUtils::CreateCube(LittleEngine::ActorPtr p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Cube",p_parent,p_name);
}
LittleEngine::ActorPtr LittleEngine::ActorUtils::CreateSphere(LittleEngine::ActorPtr p_parent, const std::string& p_name)
{
    return CreateActorWithModel("Sphere",p_parent,p_name);
}

LittleEngine::ActorPtr LittleEngine::ActorUtils::CreateEmptyActor( LittleEngine::ActorPtr p_parent, const std::string& p_name)
{
    auto& sceneManager = GetGlobalService<SceneManager>();
    const auto currentScene = sceneManager.GetCurrentScene();
    auto instance = p_name.empty() ? currentScene->CreateActor() : currentScene->CreateActor(p_name);

    if (p_parent)
        instance->SetParent(p_parent);
    return instance;
}
LittleEngine::ActorPtr LittleEngine::ActorUtils::CreateActorWithModel(const std::string& p_modelName,
                                                             LittleEngine::ActorPtr p_parent, const std::string& p_name)
{
    auto path= ":Models\\" + p_modelName + ".fbx";
    auto instance = CreateEmptyActor( p_parent, p_name);

    auto& modelManager = GetGlobalService<ResourceManagement::ModelManager>();
    auto modelRenderer = instance->AddComponent<LittleEngine::CModelRenderer>();

    const auto model = modelManager[path];
    if (model)
        modelRenderer->SetModel(model);

    auto& materialManager = GetGlobalService<ResourceManagement::MaterialManager>();
    auto materialRenderer = instance->AddComponent<LittleEngine::CMaterialRenderer>();
    const auto material = materialManager[":Materials\\Default.ovmat"];
    if (material)
        materialRenderer->FillWithMaterial(*material);
    return instance;
}
