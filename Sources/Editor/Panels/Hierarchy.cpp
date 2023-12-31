/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Panels/Hierarchy.h"
#include "../Editor/Core/EditorActions.h"

#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Selection/CheckBox.h"
#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Plugins/DDSource.h"
#include "Modules/UI/Plugins/DDTarget.h"

#include "Core/CoreInclude.h"

#include "Modules/Framework/Global/ServiceLocator.h"

#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Components/CPointLight.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CSpotLight.h"
#include "Modules/Framework/ECS/Components/CAmbientBoxLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"
#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Components/CAudioSource.h"
#include "Modules/Framework/ECS/Components/CAudioListener.h"

#include "Modules/UI/Plugins/ContextualMenu.h"

#include "../Editor/Utils/ActorCreationMenu.h"
#include "Modules/Framework/ECS/Actor.h"

namespace LittleEngine::Editor
{
    class HierarchyContextualMenu : public LittleEngine::UI::Plugins::ContextualMenu
    {
    public:
        HierarchyContextualMenu(LittleEngine::ActorPtr p_target,
                                LittleEngine::UI::Widgets::Layout::TreeNode& p_treeNode, bool p_panelMenu = false) :
            m_target(p_target),
            m_treeNode(p_treeNode)
        {
            using namespace LittleEngine::UI::Panels;
            using namespace LittleEngine::UI::Widgets;
            using namespace LittleEngine::UI::Widgets::Menu;
            using namespace LittleEngine;

            if (m_target)
            {
                auto& focusButton = CreateWidget<LittleEngine::UI::Widgets::Menu::MenuItem>("Focus");
                focusButton.ClickedEvent += [this]
                {
                    EDITOR_EXEC(MoveToTarget(m_target));
                };

                auto& duplicateButton = CreateWidget<LittleEngine::UI::Widgets::Menu::MenuItem>("Duplicate");
                duplicateButton.ClickedEvent += [this]
                {
                    EDITOR_EXEC(DelayAction(EDITOR_BIND(DuplicateActor, m_target, nullptr, true), 0));
                };

                auto& deleteButton = CreateWidget<LittleEngine::UI::Widgets::Menu::MenuItem>("Delete");
                deleteButton.ClickedEvent += [this]
                {
                    EDITOR_EXEC(DestroyActor(m_target));
                };
            }

            auto& createActor = CreateWidget<LittleEngine::UI::Widgets::Menu::MenuList>("Create...");
            Utils::ActorCreationMenu::GenerateActorCreationMenu(
                createActor, m_target, std::bind(&LittleEngine::UI::Widgets::Layout::TreeNode::Open, &m_treeNode));
        }

        virtual void Execute() override
        {
            if (m_widgets.size() > 0)
                LittleEngine::UI::Plugins::ContextualMenu::Execute();
        }

    private:
        ActorPtr m_target;
        LittleEngine::UI::Widgets::Layout::TreeNode& m_treeNode;
    };

    void ExpandTreeNode(LittleEngine::UI::Widgets::Layout::TreeNode& p_toExpand,
                        const LittleEngine::UI::Widgets::Layout::TreeNode* p_root)
    {
        p_toExpand.Open();

        if (&p_toExpand != p_root && p_toExpand.HasParent())
        {
            ExpandTreeNode(*static_cast<LittleEngine::UI::Widgets::Layout::TreeNode*>(p_toExpand.GetParent()), p_root);
        }
    }

    std::vector<LittleEngine::UI::Widgets::Layout::TreeNode*> nodesToCollapse;
    std::vector<LittleEngine::UI::Widgets::Layout::TreeNode*> founds;

    void ExpandTreeNodeAndEnable(LittleEngine::UI::Widgets::Layout::TreeNode& p_toExpand,
                                 const LittleEngine::UI::Widgets::Layout::TreeNode* p_root)
    {
        if (!p_toExpand.IsOpened())
        {
            p_toExpand.Open();
            nodesToCollapse.push_back(&p_toExpand);
        }

        p_toExpand.enabled = true;

        if (&p_toExpand != p_root && p_toExpand.HasParent())
        {
            ExpandTreeNodeAndEnable(*static_cast<LittleEngine::UI::Widgets::Layout::TreeNode*>(p_toExpand.GetParent()),
                                    p_root);
        }
    }

    Panels::Hierarchy::Hierarchy
    (
        const std::string& p_title,
        bool p_opened,
        const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
    ) : PanelWindow(p_title, p_opened, p_windowSettings)
    {
        auto& searchBar = CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>();
        searchBar.ContentChangedEvent += [this](const std::string& p_content)
        {
            founds.clear();
            auto content = p_content;
            std::transform(content.begin(), content.end(), content.begin(), ::tolower);

            for (auto& [actor, item] : m_widgetActorLink)
            {
                if (!p_content.empty())
                {
                    auto itemName = item->name;
                    std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);

                    if (itemName.find(content) != std::string::npos)
                    {
                        founds.push_back(item);
                    }

                    item->enabled = false;
                }
                else
                {
                    item->enabled = true;
                }
            }

            for (auto node : founds)
            {
                node->enabled = true;

                if (node->HasParent())
                {
                    ExpandTreeNodeAndEnable(
                        *static_cast<LittleEngine::UI::Widgets::Layout::TreeNode*>(node->GetParent()), m_sceneRoot);
                }
            }

            if (p_content.empty())
            {
                for (auto node : nodesToCollapse)
                {
                    node->Close();
                }

                nodesToCollapse.clear();
            }
        };

        m_sceneRoot = &CreateWidget<LittleEngine::UI::Widgets::Layout::TreeNode>("Root", true);
        static_cast<LittleEngine::UI::Widgets::Layout::TreeNode*>(m_sceneRoot)->Open();
        m_sceneRoot->AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<
                ActorPtr, LittleEngine::UI::Widgets::Layout::TreeNode*>>>("Actor").DataReceivedEvent += [this
            ](std::pair<ActorPtr, LittleEngine::UI::Widgets::Layout::TreeNode*> p_element)
            {
                if (p_element.second->HasParent())
                    p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

                m_sceneRoot->ConsiderWidget(*p_element.second);

                p_element.first->DetachFromParent();
            };
        m_sceneRoot->AddPlugin<HierarchyContextualMenu>(nullptr, *m_sceneRoot);

        EDITOR_EVENT(ActorUnselectedEvent) += std::bind(&Hierarchy::UnselectActorsWidgets, this);
        EDITOR_CONTEXT(sceneManager).SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);
        EDITOR_CONTEXT(sceneManager).SceneLoadEvent += std::bind(&Hierarchy::RebuildFromScene, this, std::placeholders::_1);
        LittleEngine::Scene::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
        LittleEngine::Scene::DestroyedEvent +=
            std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
        EDITOR_EVENT(ActorSelectedEvent) += std::bind(&Hierarchy::SelectActorByInstance, this, std::placeholders::_1);
        LittleEngine::Scene::AttachEvent += std::bind(&Hierarchy::AttachActorToParent, this, std::placeholders::_1);
        LittleEngine::Scene::DettachEvent += std::bind(&Hierarchy::DetachFromParent, this, std::placeholders::_1);
    }
    void Panels::Hierarchy::RebuildFromScene(Scene* p_curScene)
    {
        Clear();
        auto actors = p_curScene->GetActorsInternal();
        for (auto actor : actors)
        {
            AddActorByInstance(actor);
        }
        for (auto actor : actors)
        {
            if(actor->HasParent())
            {
                AttachActorToParent(actor);
            }
        }
    }
    void Panels::Hierarchy::Clear()
    {
        EDITOR_EXEC(UnselectActor());

        m_sceneRoot->RemoveAllWidgets();
        m_widgetActorLink.clear();
    }

    void Panels::Hierarchy::UnselectActorsWidgets()
    {
        for (auto& widget : m_widgetActorLink)
            widget.second->selected = false;
    }

    void Panels::Hierarchy::SelectActorByInstance(LittleEngine::ActorPtr p_actor)
    {
        if (auto result = m_widgetActorLink.find(p_actor->GetID()); result != m_widgetActorLink.end())
            if (result->second)
                SelectActorByWidget(*result->second);
    }

    void Panels::Hierarchy::SelectActorByWidget(
        LittleEngine::UI::Widgets::Layout::TreeNode& p_widget)
    {
        UnselectActorsWidgets();

        p_widget.selected = true;

        if (p_widget.HasParent())
        {
            ExpandTreeNode(*static_cast<LittleEngine::UI::Widgets::Layout::TreeNode*>(p_widget.GetParent()),
                           m_sceneRoot);
        }
    }

    void Panels::Hierarchy::AttachActorToParent(LittleEngine::ActorPtr p_actor)
    {
        auto actorWidget = m_widgetActorLink.find(p_actor->GetID());

        if (p_actor != nullptr && actorWidget != m_widgetActorLink.end())
        {
            auto widget = actorWidget->second;

            if (widget->HasParent())
                widget->GetParent()->UnconsiderWidget(*widget);

            if (p_actor->HasParent())
            {
                auto parentWidget = m_widgetActorLink.at(p_actor->GetParentID());
                parentWidget->leaf = false;
                parentWidget->ConsiderWidget(*widget);
            }
        }
    }

    void Panels::Hierarchy::DetachFromParent(LittleEngine::ActorPtr p_actor)
    {
        if (auto actorWidget = m_widgetActorLink.find(p_actor->GetID()); actorWidget != m_widgetActorLink.end())
        {
            if (p_actor != nullptr && p_actor->HasParent() && p_actor->GetParent()->GetChildren().size() == 1)
            {
                if (auto parentWidget = m_widgetActorLink.find(p_actor->GetParentID()); parentWidget !=
                    m_widgetActorLink.end())
                {
                    parentWidget->second->leaf = true;
                }
            }

            auto widget = actorWidget->second;

            if (widget->HasParent())
                widget->GetParent()->UnconsiderWidget(*widget);

            m_sceneRoot->ConsiderWidget(*widget);
        }
    }

    void Panels::Hierarchy::DeleteActorByInstance(LittleEngine::ActorPtr p_actor)
    {
        if (auto result = m_widgetActorLink.find(p_actor->GetID()); result != m_widgetActorLink.end())
            if (result->second)
                result->second->Destroy();
        m_widgetActorLink.erase(p_actor->GetID());
    }

    void Panels::Hierarchy::AddActorByInstance(ActorPtr p_actor)
    {
        auto& textSelectable = m_sceneRoot->CreateWidget<LittleEngine::UI::Widgets::Layout::TreeNode>(
            p_actor->GetName(), true);
        textSelectable.leaf = true;
        textSelectable.AddPlugin<HierarchyContextualMenu>(p_actor, textSelectable);
        textSelectable.AddPlugin<LittleEngine::UI::Plugins::DDSource<
            std::pair<ActorPtr, LittleEngine::UI::Widgets::Layout::TreeNode*>>>
                        ("Actor", "Attach to...",std::make_pair(p_actor, &textSelectable));
        textSelectable.AddPlugin<LittleEngine::UI::Plugins::DDTarget<
            std::pair<ActorPtr, LittleEngine::UI::Widgets::Layout::TreeNode*>>>("Actor").DataReceivedEvent +=
                    [p_actor, &textSelectable](std::pair<ActorPtr, LittleEngine::UI::Widgets::Layout::TreeNode*> p_element)
            {
                if (p_element.second->HasParent())
                    p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

                textSelectable.ConsiderWidget(*p_element.second);

                p_element.first->SetParent(p_actor);
            };
        auto& dispatcher = textSelectable.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::string>>();

        ActorPtr targetPtr = p_actor;
        dispatcher.RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

        m_widgetActorLink[targetPtr->GetID()] = &textSelectable;

        textSelectable.ClickedEvent += EDITOR_BIND(SelectActor, p_actor);
        textSelectable.DoubleClickedEvent += EDITOR_BIND(MoveToTarget, p_actor);
    }
}
