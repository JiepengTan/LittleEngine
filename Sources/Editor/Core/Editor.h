/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/LowRenderer/Camera.h"

#include "../Editor/Core/EditorRenderer.h"
#include "../Editor/Core/EditorActions.h"
#include "../Editor/Core/PanelsManager.h"

#include "Modules/UI/Modules/Canvas.h"

#include "../Editor/Core/Context.h"

#include "Core/Analytics/Profiling/ProfilerSpy.h"

namespace LittleEngine::Editor::Core
{
	/**
	* Handle the editor logic
	*/
	class Editor
	{
	public:
		/**
		* Constructor of the editor
		* @param p_context
		*/
		Editor(Context& p_context);

		/**
		* Destructor of the editor
		*/
		~Editor();

		/**
		* Handle panels creation and canvas binding
		*/
		void SetupUI();

		/**
		* Prepare the frame (Inputs update, screen clearing)
		*/
		void PreUpdate();

		/**
		* Editor main loop.
		* Render the scene, update panels...
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Handle editor global shortcuts
		*/
		void HandleGlobalShortcuts();

		/**
		* Update the current editor mode
		* @param p_deltaTime
		*/
		void UpdateCurrentEditorMode(float p_deltaTime);

		/**
		* Apply the play mode logic
		* @param p_deltaTime
		*/
		void UpdatePlayMode(float p_deltaTime);

		/**
		* Apply the edit mode logic
		* @param p_deltaTime
		*/
		void UpdateEditMode(float p_deltaTime);

		/**
		* Update editor panels
		* @param p_deltaTime
		*/
		void UpdateEditorPanels(float p_deltaTime);

		/**
		* Prepare the rendering of the views by parsing lights and sending them to the GPU
		* and filling Engine UBO with elapsed time
		* @param p_deltaTime
		*/
		void PrepareRendering(float p_deltaTime);

		/**
		* Render every views (Scene View, Game View, Asset View)
		* @param p_deltaTime
		*/
		void RenderViews(float p_deltaTime);

		/**
		* Render the editor UI using ImGui
		* @param p_deltaTime
		*/
		void RenderEditorUI(float p_deltaTime);
		
		/**
		* Actually render the scene (Buffer swapping)
		* and clear input events
		*/
		void PostUpdate();

	private:
		uint64_t m_elapsedFrames = 0;
		LittleEngine::UI::Modules::Canvas			m_canvas;
		LittleEngine::Editor::Core::Context&		m_context;
		LittleEngine::Editor::Core::EditorRenderer	m_editorRenderer;
		LittleEngine::Editor::Core::PanelsManager	m_panelsManager;
		LittleEngine::Editor::Core::EditorActions	m_editorActions;
	};
}
