/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/Loader/TextureLoader.h"
#include "Modules/Rendering/LowRenderer/Camera.h"

#include "Modules/Framework/SceneSystem/SceneManager.h"

#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/TreeNode.h"

namespace LittleEngine::Editor::Panels
{
	using namespace LittleEngine;
	class Hierarchy : public UI::Panels::PanelWindow
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
			const UI::Settings::PanelWindowSettings& p_windowSettings
		);
		
		/**
		* Clear hierarchy nodes
		*/
		void Clear();
		
		/**
		* Rebuild layout from scene hierarchy nodes
		*/
		void RebuildFromScene(Scene* p_curScene);
		/**
		* Unselect every widgets
		*/
		void UnselectActorsWidgets();

		/**
		* Select the widget corresponding to the given actor
		* @param p_actor
		*/
		void SelectActorByInstance(ActorPtr p_actor);

		/**
		* Select the widget
		* @param p_actor
		*/
		void SelectActorByWidget(UI::Widgets::Layout::TreeNode& p_widget);

		/**
		* Attach the given actor linked widget to its parent widget
		* @param p_actor
		*/
		void AttachActorToParent(ActorPtr p_actor);

		/**
		* Detach the given actor linked widget from its parent widget
		* @param p_actor
		*/
		void DetachFromParent(ActorPtr p_actor);

		/**
		* Delete the widget referencing the given actor
		* @param p_actor
		*/
		void DeleteActorByInstance(ActorPtr p_actor);

		/**
		* Add a widget referencing the given actor
		* @param p_actor
		*/
		void AddActorByInstance(ActorPtr p_actor);

		void GetActor(ActorID p_actorID);
	public:
		Eventing::Event<Actor&> ActorSelectedEvent;
		Eventing::Event<Actor&> ActorUnselectedEvent;

	private:
		UI::Widgets::Layout::TreeNode* m_sceneRoot;

		std::unordered_map<ActorID, UI::Widgets::Layout::TreeNode*> m_widgetActorLink;
	};
}
