/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Analytics/Profiling/ProfilerSpy.h"
#include "Modules/Physics/Core/PhysicsEngine.h"

#include "../Editor/Core/Editor.h"

#include "Core/Base/Application.h"
#include "../Editor/Panels/MenuBar.h"
#include "../Editor/Panels/AssetBrowser.h"
#include "../Editor/Panels/HardwareInfo.h"
#include "../Editor/Panels/Profiler.h"
#include "../Editor/Panels/Console.h"
#include "../Editor/Panels/Inspector.h"
#include "../Editor/Panels/Hierarchy.h"
#include "../Editor/Panels/SceneView.h"
#include "../Editor/Panels/GameView.h"
#include "../Editor/Panels/AssetView.h"
#include "../Editor/Panels/MaterialEditor.h"
#include "../Editor/Panels/ProjectSettings.h"
#include "../Editor/Panels/AssetProperties.h"

using namespace LittleEngine::ResourceManagement;
using namespace LittleEditor::Panels;
using namespace LittleEngine::Rendering::Resources::Loaders;
using namespace LittleEngine::Rendering::Resources::Parsers;

LittleEditor::Core::Editor::Editor(Context& p_context) : 
	m_context(p_context),
	m_editorRenderer(p_context),
	m_panelsManager(m_canvas),
	m_editorActions(m_context, m_editorRenderer, m_panelsManager)
{
	SetupUI();

	m_context.sceneManager.LoadEmptyLightedScene();
}

LittleEditor::Core::Editor::~Editor()
{
	m_context.sceneManager.UnloadCurrentScene();
}

void LittleEditor::Core::Editor::SetupUI()
{
	LittleEngine::UI::Settings::PanelWindowSettings settings;
	settings.closable = true;
	settings.collapsable = true;
	settings.dockable = true;

	m_panelsManager.CreatePanel<LittleEditor::Panels::MenuBar>("Menu Bar");
	m_panelsManager.CreatePanel<LittleEditor::Panels::AssetBrowser>("Asset Browser", true, settings, m_context.engineAssetsPath, m_context.projectAssetsPath, m_context.projectScriptsPath);
	m_panelsManager.CreatePanel<LittleEditor::Panels::HardwareInfo>("Hardware Info", false, settings, 0.2f, 50);
	m_panelsManager.CreatePanel<LittleEditor::Panels::Profiler>("Profiler", true, settings, 0.25f);
	m_panelsManager.CreatePanel<LittleEditor::Panels::Console>("Console", true, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::Hierarchy>("Hierarchy", true, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::Inspector>("Inspector", true, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::SceneView>("Scene View", true, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::GameView>("Game View", true, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::AssetView>("Asset View", false, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::MaterialEditor>("Material Editor", false, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::ProjectSettings>("Project Settings", false, settings);
	m_panelsManager.CreatePanel<LittleEditor::Panels::AssetProperties>("Asset Properties", false, settings);

	m_canvas.MakeDockspace(true);
	m_context.uiManager->SetCanvas(m_canvas);
}

void LittleEditor::Core::Editor::PreUpdate()
{
	PROFILER_SPY("Editor Pre-Update");

	m_context.device->PollEvents();
	m_context.renderer->SetClearColor(0.f, 0.f, 0.f);
	m_context.renderer->Clear();
}

void LittleEditor::Core::Editor::Update(float p_deltaTime)
{
	HandleGlobalShortcuts();
	UpdateCurrentEditorMode(p_deltaTime);
	PrepareRendering(p_deltaTime);
	UpdateEditorPanels(p_deltaTime);
	RenderViews(p_deltaTime);
	RenderEditorUI(p_deltaTime);
	m_editorActions.ExecuteDelayedActions();
}

void LittleEditor::Core::Editor::HandleGlobalShortcuts()
{
	// If the [Del] key is pressed while an actor is selected and the Scene View or Hierarchy is focused
	if (m_context.inputManager->IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_DELETE) && EDITOR_EXEC(IsAnyActorSelected()) && (EDITOR_PANEL(SceneView, "Scene View").IsFocused() || EDITOR_PANEL(Hierarchy, "Hierarchy").IsFocused()))
	{
		EDITOR_EXEC(DestroyActor(EDITOR_EXEC(GetSelectedActor())));
	}
	
}

void LittleEditor::Core::Editor::UpdateCurrentEditorMode(float p_deltaTime)
{
	if (auto editorMode = m_editorActions.GetCurrentEditorMode(); editorMode == EditorActions::EEditorMode::PLAY || editorMode == EditorActions::EEditorMode::FRAME_BY_FRAME)
		UpdatePlayMode(p_deltaTime);
	else
		UpdateEditMode(p_deltaTime);

	{
		PROFILER_SPY("Scene garbage collection");
		m_context.sceneManager.GetCurrentScene()->CollectGarbages();
		m_context.sceneManager.Update();
		auto currentScene = m_context.sceneManager.GetCurrentScene();
		if(currentScene != nullptr)
		{
			auto actor = &EDITOR_EXEC(GetSelectedActor());
			if(actor != nullptr && actor->IsAlive())
			{
				auto comps = actor->GetComponents();
				for (auto comp :comps)
				{
					if(comp->owner->IsAlive() && comp->IsUpdateInEdit)
					{
						comp->OnUpdate(p_deltaTime);
					}
				}
			}
		}
	}
}

void LittleEditor::Core::Editor::UpdatePlayMode(float p_deltaTime)
{
	auto currentScene = m_context.sceneManager.GetCurrentScene();
	bool simulationApplied = false;

	{
		PROFILER_SPY("Physics Update");
		simulationApplied = m_context.physicsEngine->Update(p_deltaTime);
	}

	if (simulationApplied)
	{
		PROFILER_SPY("FixedUpdate");
		currentScene->FixedUpdate(p_deltaTime);
	}

	{
		PROFILER_SPY("Update");
		currentScene->Update(p_deltaTime);
	}

	{
		PROFILER_SPY("LateUpdate");
		currentScene->LateUpdate(p_deltaTime);
	}

	{
		PROFILER_SPY("Audio Update");
		m_context.audioEngine->Update();
	}

	ImGui::GetIO().DisableMouseUpdate = m_context.window->GetCursorMode() == LittleEngine::Windowing::Cursor::ECursorMode::DISABLED;

	if (m_editorActions.GetCurrentEditorMode() == EditorActions::EEditorMode::FRAME_BY_FRAME)
		m_editorActions.PauseGame();

	if (m_context.inputManager->IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_ESCAPE))
		m_editorActions.StopPlaying();
}

void LittleEditor::Core::Editor::UpdateEditMode(float p_deltaTime)
{
	if (m_context.inputManager->IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_F5))
		m_editorActions.StartPlaying();
}

void LittleEditor::Core::Editor::UpdateEditorPanels(float p_deltaTime)
{
	auto& menuBar = m_panelsManager.GetPanelAs<LittleEditor::Panels::MenuBar>("Menu Bar");
	auto& profiler = m_panelsManager.GetPanelAs<LittleEditor::Panels::Profiler>("Profiler");
	auto& hardwareInfo = m_panelsManager.GetPanelAs<LittleEditor::Panels::HardwareInfo>("Hardware Info");
	auto& sceneView = m_panelsManager.GetPanelAs<LittleEditor::Panels::SceneView>("Scene View");

	menuBar.HandleShortcuts(p_deltaTime);

	if (m_elapsedFrames == 1) // Let the first frame happen and then make the scene view the first seen view
		sceneView.Focus();

	if (profiler.IsOpened())
	{
		PROFILER_SPY("Profiler Update");
		profiler.Update(p_deltaTime);
	}

	if (hardwareInfo.IsOpened())
	{
		PROFILER_SPY("Hardware Info Update");
		hardwareInfo.Update(p_deltaTime);
	}
}

void LittleEditor::Core::Editor::PrepareRendering(float p_deltaTime)
{
	PROFILER_SPY("Engine UBO Update");
	m_context.engineUBO->SetSubData(m_context.device->GetElapsedTime(), 3 * sizeof(LittleEngine::FMatrix4) + sizeof(LittleEngine::FVector3));
}

void LittleEditor::Core::Editor::RenderViews(float p_deltaTime)
{
	auto& assetView = m_panelsManager.GetPanelAs<LittleEditor::Panels::AssetView>("Asset View");
	auto& sceneView = m_panelsManager.GetPanelAs<LittleEditor::Panels::SceneView>("Scene View");
	auto& gameView = m_panelsManager.GetPanelAs<LittleEditor::Panels::GameView>("Game View");

	{
		PROFILER_SPY("Editor Views Update");

		assetView.Update(p_deltaTime);
		gameView.Update(p_deltaTime);
		sceneView.Update(p_deltaTime);
	}

	if (assetView.IsOpened())
	{
		PROFILER_SPY("Asset View Rendering");

		m_context.simulatedLightSSBO->Bind(0);
		assetView.Render();
		m_context.simulatedLightSSBO->Unbind();
	}

	m_context.lightSSBO->Bind(0);

	if (gameView.IsOpened())
	{
		PROFILER_SPY("Game View Rendering");

		gameView.Render();
	}

	if (sceneView.IsOpened())
	{
		PROFILER_SPY("Scene View Rendering");

		sceneView.Render();
	}

	m_context.lightSSBO->Unbind();
}

void LittleEditor::Core::Editor::RenderEditorUI(float p_deltaTime)
{
	PROFILER_SPY("Editor UI Rendering");

	m_editorActions.GetRenderer().RenderUI();
}

void LittleEditor::Core::Editor::PostUpdate()
{
	PROFILER_SPY("Editor Post-Update");

	m_context.window->SwapBuffers();
	m_context.inputManager->ClearEvents();
	++m_elapsedFrames;
}
