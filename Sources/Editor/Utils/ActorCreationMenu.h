/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <functional>

namespace LittleEngine::UI::Widgets::Menu
{
    class MenuList;
}

namespace LittleEngine
{
    class Actor;
}

namespace LittleEngine::Editor::Utils
{
    /**
    * Class exposing tools to generate an actor creation menu
    */
    class ActorCreationMenu
    {
    public:
        /**
        * Disabled constructor
        */
        ActorCreationMenu() = delete;

        /**
        * Generates an actor creation menu under the given MenuList item.
        * Also handles custom additionnal OnClick callback
        * @param p_menuList
        * @param p_parent
        * @param p_onItemClicked
        */
        static void GenerateActorCreationMenu(LittleEngine::UI::Widgets::Menu::MenuList& p_menuList, ActorPtr p_parent = nullptr, std::optional<std::function<void()>> p_onItemClicked = {});
    };
}
