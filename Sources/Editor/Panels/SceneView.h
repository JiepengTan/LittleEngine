/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Editor/Panels/AViewControllable.h"
#include "../Editor/Core/GizmoBehaviour.h"

namespace LittleEditor::Panels
{
	class SceneView : public LittleEditor::Panels::AViewControllable
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
		LittleEngine::SceneSystem::SceneManager& m_sceneManager;
		LittleEngine::Rendering::Buffers::Framebuffer m_actorPickingFramebuffer;
		LittleEditor::Core::GizmoBehaviour m_gizmoOperations;
		LittleEditor::Core::EGizmoOperation m_currentOperation = LittleEditor::Core::EGizmoOperation::TRANSLATE;

		std::optional<std::reference_wrapper<LittleEngine::Actor>> m_highlightedActor;
		std::optional<LittleEditor::Core::GizmoBehaviour::EDirection> m_highlightedGizmoDirection;
	};
}
