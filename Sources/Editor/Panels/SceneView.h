/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Editor/Panels/AViewControllable.h"
#include "../Editor/Core/GizmoBehaviour.h"

namespace LittleEngine::Editor::Panels
{
	class SceneView : public LittleEngine::Editor::Panels::AViewControllable
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		SceneView
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the scene view
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

		/**
		* Render the actual scene
		* @param p_defaultRenderState
		*/
		void RenderScene(uint8_t p_defaultRenderState);

		/**
		* Render the scene for actor picking (Using unlit colors)
		*/
		void RenderSceneForActorPicking();

		/**
		* Render the scene for actor picking and handle the logic behind it
		*/
		void HandleActorPicking();

	private:
		LittleEngine::SceneManager& m_sceneManager;
		LittleEngine::Rendering::Buffers::Framebuffer m_actorPickingFramebuffer;
		LittleEngine::Editor::Core::GizmoBehaviour m_gizmoOperations;
		LittleEngine::Editor::Core::EGizmoOperation m_currentOperation = LittleEngine::Editor::Core::EGizmoOperation::TRANSLATE;

		std::optional<ActorPtr> m_highlightedActor;
		std::optional<LittleEngine::Editor::Core::GizmoBehaviour::EDirection> m_highlightedGizmoDirection;
	};
}
