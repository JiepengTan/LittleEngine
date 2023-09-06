/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Menu/MenuList.h"
#include "Modules/UI/Widgets/Menu/MenuItem.h"

#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Components/CPointLight.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CSpotLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"
#include "Modules/Framework/ECS/Components/CAudioSource.h"
#include "Modules/Framework/ECS/Components/CAudioListener.h"
#include "Modules/Framework/ECS/Components/CCamera.h"

#include "../Editor/Core/EditorActions.h"
#include "../Editor/Utils/ActorCreationMenu.h"

std::function<void()> Combine(std::function<void()> p_a, std::optional<std::function<void()>> p_b)
{
    if (p_b.has_value())
    {
        return [=]()
        {
            p_a();
            p_b.value()();
        };
    }

    return p_a;
}

template<class T>
std::function<void()> ActorWithComponentCreationHandler(LittleEngine::ActorPtr p_parent, std::optional<std::function<void()>> p_onItemClicked)
{
    return Combine(EDITOR_BIND(CreateMonoComponentActor<T>, true, p_parent), p_onItemClicked);
}

std::function<void()> ActorWithModelComponentCreationHandler(LittleEngine::ActorPtr p_parent, const std::string& p_modelName, std::optional<std::function<void()>> p_onItemClicked)
{
    return Combine(EDITOR_BIND(CreateActorWithModel, ":Models\\" + p_modelName + ".fbx", true, p_parent, p_modelName), p_onItemClicked);
}

void LittleEngine::Editor::Utils::ActorCreationMenu::GenerateActorCreationMenu(LittleEngine::UI::Widgets::Menu::MenuList& p_menuList, ActorPtr p_parent, std::optional<std::function<void()>> p_onItemClicked)
{
    using namespace LittleEngine::UI::Widgets::Menu;
    using namespace LittleEngine;

    p_menuList.CreateWidget<MenuItem>("Create Empty").ClickedEvent += Combine(EDITOR_BIND(CreateEmptyActor, true, p_parent, ""), p_onItemClicked);

    auto& primitives = p_menuList.CreateWidget<MenuList>("Primitives");
    auto& physicals = p_menuList.CreateWidget<MenuList>("Physicals");
    auto& lights = p_menuList.CreateWidget<MenuList>("Lights");
    auto& audio = p_menuList.CreateWidget<MenuList>("Audio");
    auto& others = p_menuList.CreateWidget<MenuList>("Others");

    primitives.CreateWidget<MenuItem>("Cube").ClickedEvent              += ActorWithModelComponentCreationHandler(p_parent, "Cube", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Sphere").ClickedEvent            += ActorWithModelComponentCreationHandler(p_parent, "Sphere", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Cone").ClickedEvent              += ActorWithModelComponentCreationHandler(p_parent, "Cone", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Cylinder").ClickedEvent          += ActorWithModelComponentCreationHandler(p_parent, "Cylinder", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Plane").ClickedEvent             += ActorWithModelComponentCreationHandler(p_parent, "Plane", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Gear").ClickedEvent              += ActorWithModelComponentCreationHandler(p_parent, "Gear", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Helix").ClickedEvent             += ActorWithModelComponentCreationHandler(p_parent, "Helix", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Pipe").ClickedEvent              += ActorWithModelComponentCreationHandler(p_parent, "Pipe", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Pyramid").ClickedEvent           += ActorWithModelComponentCreationHandler(p_parent, "Pyramid", p_onItemClicked);
    primitives.CreateWidget<MenuItem>("Torus").ClickedEvent             += ActorWithModelComponentCreationHandler(p_parent, "Torus", p_onItemClicked);
    physicals.CreateWidget<MenuItem>("Physical Box").ClickedEvent       += ActorWithComponentCreationHandler<CPhysicalBox>(p_parent, p_onItemClicked);
    physicals.CreateWidget<MenuItem>("Physical Sphere").ClickedEvent    += ActorWithComponentCreationHandler<CPhysicalSphere>(p_parent, p_onItemClicked);
    physicals.CreateWidget<MenuItem>("Physical Capsule").ClickedEvent   += ActorWithComponentCreationHandler<CPhysicalCapsule>(p_parent, p_onItemClicked);
    lights.CreateWidget<MenuItem>("Point").ClickedEvent                 += ActorWithComponentCreationHandler<CPointLight>(p_parent, p_onItemClicked);
    lights.CreateWidget<MenuItem>("Directional").ClickedEvent           += ActorWithComponentCreationHandler<CDirectionalLight>(p_parent, p_onItemClicked);
    lights.CreateWidget<MenuItem>("Spot").ClickedEvent                  += ActorWithComponentCreationHandler<CSpotLight>(p_parent, p_onItemClicked);
    lights.CreateWidget<MenuItem>("Ambient Box").ClickedEvent           += ActorWithComponentCreationHandler<CAmbientBoxLight>(p_parent, p_onItemClicked);
    lights.CreateWidget<MenuItem>("Ambient Sphere").ClickedEvent        += ActorWithComponentCreationHandler<CAmbientSphereLight>(p_parent, p_onItemClicked);
    audio.CreateWidget<MenuItem>("Audio Source").ClickedEvent           += ActorWithComponentCreationHandler<CAudioSource>(p_parent, p_onItemClicked);
    audio.CreateWidget<MenuItem>("Audio Listener").ClickedEvent         += ActorWithComponentCreationHandler<CAudioListener>(p_parent, p_onItemClicked);
    others.CreateWidget<MenuItem>("Camera").ClickedEvent                += ActorWithComponentCreationHandler<CCamera>(p_parent, p_onItemClicked);
}
