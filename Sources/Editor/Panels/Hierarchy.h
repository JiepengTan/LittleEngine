/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Resources/Loaders/TextureLoader.h"
#include "Modules/Rendering/LowRenderer/Camera.h"

#include "Modules/Framework/SceneSystem/SceneManager.h"

#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/TreeNode.h"

namespace LittleEditor::Panels
{
	class Hierarchy : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Hierarchy
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);
		
		/**
		* Clear hierarchy nodes
		*/
		void Clear();

		/**
		* Unselect every widgets
		*/
		void UnselectActorsWidgets();

		/**
		* Select the widget corresponding to the given actor
		* @param p_actor
		*/
		void SelectActorByInstance(LittleEngine::Actor& p_actor);

		/**
		* Select the widget
		* @param p_actor
		*/
		void SelectActorByWidget(LittleEngine::UI::Widgets::Layout::TreeNode& p_widget);

		/**
		* Attach the given actor linked widget to its parent widget
		* @param p_actor
		*/
		void AttachActorToParent(LittleEngine::Actor& p_actor);

		/**
		* Detach the given actor linked widget from its parent widget
		* @param p_actor
		*/
		void DetachFromParent(LittleEngine::Actor& p_actor);

		/**
		* Delete the widget referencing the given actor
		* @param p_actor
		*/
		void DeleteActorByInstance(LittleEngine::Actor& p_actor);

		/**
		* Add a widget referencing the given actor
		* @param p_actor
		*/
		void AddActorByInstance(LittleEngine::Actor& p_actor);

	public:
		LittleEngine::Eventing::Event<LittleEngine::Actor&> ActorSelectedEvent;
		LittleEngine::Eventing::Event<LittleEngine::Actor&> ActorUnselectedEvent;

	private:
		LittleEngine::UI::Widgets::Layout::TreeNode* m_sceneRoot;

		std::unordered_map<LittleEngine::Actor*, LittleEngine::UI::Widgets::Layout::TreeNode*> m_widgetActorLink;
	};
}
