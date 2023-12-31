/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Utils/SystemCalls.h"

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

#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Widgets/Sliders/SliderInt.h"
#include "Modules/UI/Widgets/Sliders/SliderFloat.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"

#include "../Editor/Panels/MenuBar.h"
#include "../Editor/Panels/SceneView.h"
#include "../Editor/Panels/AssetView.h"
#include "../Editor/Core/EditorActions.h"
#include "../Editor/Settings/EditorSettings.h"
#include "../Editor/Utils/ActorCreationMenu.h"


using namespace LittleEngine::UI::Panels;
using namespace LittleEngine::UI::Widgets;
using namespace LittleEngine::UI::Widgets::Menu;
using namespace LittleEngine;

LittleEngine::Editor::Panels::MenuBar::MenuBar()
{
	CreateFileMenu();
	CreateEditMenu();
	CreateWindowMenu();
	CreateActorsMenu();
	CreateResourcesMenu();
	CreateSettingsMenu();
	CreateLayoutMenu();
	CreateHelpMenu();
	CreateToolbar();
}
LittleEngine::UI::Widgets::Buttons::ButtonImage* LittleEngine::Editor::Panels::MenuBar::CreateToolbarItem(std::string p_name,float& p_offset)
{
	const int btnOffset = 20;
	const int btnSize = 16;
	const auto btn = &CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonImage>(
		EDITOR_CONTEXT(editorResources)->GetTexture(p_name)->id, LittleEngine::FVector2{ btnSize, btnSize });
	btn->isAbsoluteOffset = true;
	btn->offset = LittleEngine::FVector2(p_offset,0);
	p_offset += btnSize + btnOffset;
	return btn;
}
void LittleEngine::Editor::Panels::MenuBar::CreateToolbar()
{
	float startOffset = 700;
	m_playButton	= CreateToolbarItem("Button_Play",startOffset); 
	m_pauseButton	= CreateToolbarItem("Button_Pause",startOffset);
	m_nextButton	= CreateToolbarItem("Button_Next",startOffset);
	auto refreshButton	= CreateToolbarItem("Button_Refresh",startOffset);
	
	m_playButton->lineBreak		= false;
	m_pauseButton->lineBreak	= false;
	m_nextButton->lineBreak		= false;
	refreshButton->lineBreak		= false;

	m_playButton->ClickedEvent	+=  [this]()
	{
		auto& editorActors = GetGlobalService<LittleEngine::Editor::Core::EditorActions>();
		if (editorActors.GetCurrentEditorMode() == LittleEngine::Editor::Core::EditorActions::EEditorMode::EDIT)
		{
			editorActors.StartPlaying();
			m_playButton->textureID.id =  (uint32_t)EDITOR_CONTEXT(editorResources)->GetTexture("Button_Stop")->id;
		}else
		{
			editorActors.StopPlaying();
			m_playButton->textureID.id =  (uint32_t)EDITOR_CONTEXT(editorResources)->GetTexture("Button_Play")->id;
		}
	};
	m_pauseButton->ClickedEvent	+=  [this]()
	{
		auto& editorActors = GetGlobalService<LittleEngine::Editor::Core::EditorActions>();
		if (editorActors.GetCurrentEditorMode() == LittleEngine::Editor::Core::EditorActions::EEditorMode::PLAY)
		{
			editorActors.PauseGame();
			m_pauseButton->tint = { 0.9, 0.5, 0.5, 1 };
		}else if (editorActors.GetCurrentEditorMode() == LittleEngine::Editor::Core::EditorActions::EEditorMode::PAUSE)
		{
			editorActors.ResumeGame();
			m_pauseButton->tint = { 1, 1, 1, 1 };
		}
	};
	m_nextButton->ClickedEvent	+= EDITOR_BIND(NextFrame);
	refreshButton->ClickedEvent	+= EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](LittleEngine::Editor::Core::EditorActions::EEditorMode p_newMode)
	{
		auto enable = [](LittleEngine::UI::Widgets::Buttons::ButtonImage* p_button, bool p_enable)
		{
			p_button->disabled = !p_enable;
			p_button->tint = p_enable ? LittleEngine::Color{ 1.0f, 1.0f, 1.0f, 1.0f} : LittleEngine::Color{1.0f, 1.0f, 1.0f, 0.15f};
		};

		switch (p_newMode)
		{
		case LittleEngine::Editor::Core::EditorActions::EEditorMode::EDIT:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_nextButton, false);
			break;
		case LittleEngine::Editor::Core::EditorActions::EEditorMode::PLAY:
			enable(m_playButton, true);
			enable(m_pauseButton, true);
			enable(m_nextButton, true);
			break;
		case LittleEngine::Editor::Core::EditorActions::EEditorMode::PAUSE:
			enable(m_playButton, true);
			enable(m_pauseButton, true);
			enable(m_nextButton, true);
			break;
		case LittleEngine::Editor::Core::EditorActions::EEditorMode::FRAME_BY_FRAME:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_nextButton, true);
			break;
		}
	};

	EDITOR_EXEC(SetEditorMode(LittleEngine::Editor::Core::EditorActions::EEditorMode::EDIT));
}


void LittleEngine::Editor::Panels::MenuBar::HandleShortcuts(float p_deltaTime)
{
	auto& inputManager = *EDITOR_CONTEXT(inputManager);

	if (inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_LEFT_CONTROL) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN)
	{
		if (inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_N))
			EDITOR_EXEC(LoadEmptyScene());

		if (inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_S))
		{
			if (inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_LEFT_SHIFT) == LittleEngine::Windowing::Inputs::EKeyState::KEY_UP)
				EDITOR_EXEC(SaveSceneChanges());
			else
				EDITOR_EXEC(SaveAs());
		}
		
		if (inputManager.IsKeyPressed(LittleEngine::Windowing::Inputs::EKey::KEY_B))
		{
			bool isShiftPress = inputManager.GetKeyState(LittleEngine::Windowing::Inputs::EKey::KEY_LEFT_SHIFT) == LittleEngine::Windowing::Inputs::EKeyState::KEY_DOWN;
			EDITOR_EXEC(Build(!isShiftPress,true));
		}
	}
}

void LittleEngine::Editor::Panels::MenuBar::CreateFileMenu()
{
	auto& fileMenu = CreateWidget<MenuList>("File");
	fileMenu.CreateWidget<MenuItem>("New Scene", "CTRL + N").ClickedEvent					+= EDITOR_BIND(LoadEmptyScene);
	fileMenu.CreateWidget<MenuItem>("Save Scene", "CTRL + S").ClickedEvent					+= EDITOR_BIND(SaveSceneChanges);
	fileMenu.CreateWidget<MenuItem>("Save Scene As...", "CTRL + SHIFT + S").ClickedEvent	+= EDITOR_BIND(SaveAs);
	
	
	fileMenu.CreateWidget<MenuItem>("Build game", "CTRL + SHIFT + B").ClickedEvent			+= EDITOR_BIND(Build, false, false);
	fileMenu.CreateWidget<MenuItem>("Build game and run", "CTRL + B").ClickedEvent			+= EDITOR_BIND(Build, true, false);
	fileMenu.CreateWidget<Visual::Separator>();
	fileMenu.CreateWidget<MenuItem>("Temporary build").ClickedEvent							  += EDITOR_BIND(Build, true, true);
	fileMenu.CreateWidget<Visual::Separator>();
	fileMenu.CreateWidget<MenuItem>("Exit", "ALT + F4").ClickedEvent						+= [] { EDITOR_CONTEXT(window)->SetShouldClose(true); };

}

void LittleEngine::Editor::Panels::MenuBar::CreateEditMenu()
{
	m_editMenu = &CreateWidget<MenuList>("Edit");
	m_editMenu->ClickedEvent += [this]()
	{
		if(m_panels.count(PorjectSettingName) > 0)
		{
			auto& panel= m_panels.at(PorjectSettingName);
			panel.second.get().checked = panel.first.get().IsOpened();
		}
	};
}

void LittleEngine::Editor::Panels::MenuBar::CreateWindowMenu()
{
	m_windowMenu = &CreateWidget<MenuList>("Window");
	m_windowMenu->CreateWidget<MenuItem>("Close all").ClickedEvent	+= std::bind(&MenuBar::OpenEveryWindows, this, false);
	m_windowMenu->CreateWidget<MenuItem>("Open all").ClickedEvent		+= std::bind(&MenuBar::OpenEveryWindows, this, true);
	m_windowMenu->CreateWidget<Visual::Separator>();

	/* When the menu is opened, we update which window is marked as "Opened" or "Closed" */
	m_windowMenu->ClickedEvent += std::bind(&MenuBar::UpdateToggleableItems, this);
}

void LittleEngine::Editor::Panels::MenuBar::CreateActorsMenu()
{
	auto& actorsMenu = CreateWidget<MenuList>("Actors");
    Utils::ActorCreationMenu::GenerateActorCreationMenu(actorsMenu);
}

void LittleEngine::Editor::Panels::MenuBar::CreateResourcesMenu()
{
	auto& resourcesMenu = CreateWidget<MenuList>("Resources");
	resourcesMenu.CreateWidget<MenuItem>("Compile shaders").ClickedEvent += EDITOR_BIND(CompileShaders);
	resourcesMenu.CreateWidget<MenuItem>("Save materials").ClickedEvent += EDITOR_BIND(SaveMaterials);
}

void LittleEngine::Editor::Panels::MenuBar::CreateSettingsMenu()
{
	auto& settingsMenu = CreateWidget<MenuList>("Settings");
	settingsMenu.CreateWidget<MenuItem>("Spawn actors at origin", "", true, true).ValueChangedEvent		+= EDITOR_BIND(SetActorSpawnAtOrigin, std::placeholders::_1);
	settingsMenu.CreateWidget<MenuItem>("Vertical Synchronization", "", true, true).ValueChangedEvent	+= [this](bool p_value) { EDITOR_CONTEXT(device)->SetVsync(p_value); };
	auto& cameraSpeedMenu = settingsMenu.CreateWidget<MenuList>("Camera Speed");
	cameraSpeedMenu.CreateWidget<LittleEngine::UI::Widgets::Sliders::SliderInt>(1, 50, 15, LittleEngine::UI::Widgets::Sliders::ESliderOrientation::HORIZONTAL, "Scene View").ValueChangedEvent += EDITOR_BIND(SetSceneViewCameraSpeed, std::placeholders::_1);
	cameraSpeedMenu.CreateWidget<LittleEngine::UI::Widgets::Sliders::SliderInt>(1, 50, 15, LittleEngine::UI::Widgets::Sliders::ESliderOrientation::HORIZONTAL, "Asset View").ValueChangedEvent += EDITOR_BIND(SetAssetViewCameraSpeed, std::placeholders::_1);
	auto& cameraPositionMenu = settingsMenu.CreateWidget<MenuList>("Reset Camera");
	cameraPositionMenu.CreateWidget<MenuItem>("Scene View").ClickedEvent += EDITOR_BIND(ResetSceneViewCameraPosition);
	cameraPositionMenu.CreateWidget<MenuItem>("Asset View").ClickedEvent += EDITOR_BIND(ResetAssetViewCameraPosition);

	auto& viewColors = settingsMenu.CreateWidget<MenuList>("View Colors");
	auto& sceneViewBackground = viewColors.CreateWidget<MenuList>("Scene View Background");
	auto& sceneViewBackgroundPicker = sceneViewBackground.CreateWidget<Selection::ColorEdit>(false, LittleEngine::Color{ 0.098f, 0.098f, 0.098f });
	sceneViewBackgroundPicker.ColorChangedEvent += [this](const auto & color)
	{
		EDITOR_PANEL(Panels::SceneView, "Scene View").GetCamera().SetClearColor({ color.r, color.g, color.b });
	};
	sceneViewBackground.CreateWidget<MenuItem>("Reset").ClickedEvent += [this, &sceneViewBackgroundPicker]
	{
		EDITOR_PANEL(Panels::SceneView, "Scene View").GetCamera().SetClearColor({ 0.098f, 0.098f, 0.098f });
		sceneViewBackgroundPicker.color = { 0.098f, 0.098f, 0.098f };
	};

	auto& sceneViewGrid = viewColors.CreateWidget<MenuList>("Scene View Grid");
    auto& sceneViewGridPicker = sceneViewGrid.CreateWidget<Selection::ColorEdit>(false, LittleEngine::Color(0.176f, 0.176f, 0.176f));
	sceneViewGridPicker.ColorChangedEvent += [this](const auto & color)
	{
		EDITOR_PANEL(Panels::SceneView, "Scene View").SetGridColor({ color.r, color.g, color.b });
	};
	sceneViewGrid.CreateWidget<MenuItem>("Reset").ClickedEvent += [this, &sceneViewGridPicker]
	{
		EDITOR_PANEL(Panels::SceneView, "Scene View").SetGridColor(LittleEngine::FVector3(0.176f, 0.176f, 0.176f));
		sceneViewGridPicker.color = LittleEngine::Color(0.176f, 0.176f, 0.176f);
	};

	auto& assetViewBackground = viewColors.CreateWidget<MenuList>("Asset View Background");
	auto& assetViewBackgroundPicker = assetViewBackground.CreateWidget<Selection::ColorEdit>(false, LittleEngine::Color{ 0.098f, 0.098f, 0.098f });
	assetViewBackgroundPicker.ColorChangedEvent += [this](const auto & color)
	{
		EDITOR_PANEL(Panels::AssetView, "Asset View").GetCamera().SetClearColor({ color.r, color.g, color.b });
	};
	assetViewBackground.CreateWidget<MenuItem>("Reset").ClickedEvent += [this, &assetViewBackgroundPicker]
	{
		EDITOR_PANEL(Panels::AssetView, "Asset View").GetCamera().SetClearColor({ 0.098f, 0.098f, 0.098f });
		assetViewBackgroundPicker.color = { 0.098f, 0.098f, 0.098f };
	};

	auto& assetViewGrid = viewColors.CreateWidget<MenuList>("Asset View Grid");
	auto& assetViewGridPicker = assetViewGrid.CreateWidget<Selection::ColorEdit>(false, LittleEngine::Color(0.176f, 0.176f, 0.176f));
	assetViewGridPicker.ColorChangedEvent += [this](const auto & color)
	{
		EDITOR_PANEL(Panels::AssetView, "Asset View").SetGridColor({ color.r, color.g, color.b });
	};
	assetViewGrid.CreateWidget<MenuItem>("Reset").ClickedEvent += [this, &assetViewGridPicker]
	{
		EDITOR_PANEL(Panels::AssetView, "Asset View").SetGridColor(LittleEngine::FVector3(0.176f, 0.176f, 0.176f));
		assetViewGridPicker.color = LittleEngine::Color(0.176f, 0.176f, 0.176f);
	};

	auto& sceneViewBillboardScaleMenu = settingsMenu.CreateWidget<MenuList>("3D Icons Scales");
	auto& lightBillboardScaleSlider = sceneViewBillboardScaleMenu.CreateWidget<Sliders::SliderInt>(0, 100, static_cast<int>(Settings::EditorSettings::LightBillboardScale * 100.0f), LittleEngine::UI::Widgets::Sliders::ESliderOrientation::HORIZONTAL, "Lights");
	lightBillboardScaleSlider.ValueChangedEvent += [this](int p_value) { Settings::EditorSettings::LightBillboardScale = p_value / 100.0f; };
	lightBillboardScaleSlider.format = "%d %%";

	auto& snappingMenu = settingsMenu.CreateWidget<MenuList>("Snapping");
	snappingMenu.CreateWidget<Drags::DragFloat>(0.001f, 999999.0f, Settings::EditorSettings::TranslationSnapUnit, 0.05f, "Translation Unit").ValueChangedEvent += [this](float p_value) { Settings::EditorSettings::TranslationSnapUnit = p_value; };
	snappingMenu.CreateWidget<Drags::DragFloat>(0.001f, 999999.0f, Settings::EditorSettings::RotationSnapUnit, 1.0f, "Rotation Unit").ValueChangedEvent += [this](float p_value) { Settings::EditorSettings::RotationSnapUnit = p_value; };
	snappingMenu.CreateWidget<Drags::DragFloat>(0.001f, 999999.0f, Settings::EditorSettings::ScalingSnapUnit, 0.05f, "Scaling Unit").ValueChangedEvent += [this](float p_value) { Settings::EditorSettings::ScalingSnapUnit = p_value; };

	auto& debuggingMenu = settingsMenu.CreateWidget<MenuList>("Debugging");
	debuggingMenu.CreateWidget<MenuItem>("Show geometry bounds", "", true, Settings::EditorSettings::ShowGeometryBounds).ValueChangedEvent += [this](bool p_value) { Settings::EditorSettings::ShowGeometryBounds = p_value; };
	debuggingMenu.CreateWidget<MenuItem>("Show lights bounds", "", true, Settings::EditorSettings::ShowLightBounds).ValueChangedEvent += [this](bool p_value) { Settings::EditorSettings::ShowLightBounds = p_value; };
	auto& subMenu = debuggingMenu.CreateWidget<MenuList>("Frustum culling visualizer...");
	subMenu.CreateWidget<MenuItem>("For geometry", "", true, Settings::EditorSettings::ShowGeometryFrustumCullingInSceneView).ValueChangedEvent += [this](bool p_value) { Settings::EditorSettings::ShowGeometryFrustumCullingInSceneView = p_value; };
	subMenu.CreateWidget<MenuItem>("For lights", "", true, Settings::EditorSettings::ShowLightFrustumCullingInSceneView).ValueChangedEvent += [this](bool p_value) { Settings::EditorSettings::ShowLightFrustumCullingInSceneView = p_value; };
}

void LittleEngine::Editor::Panels::MenuBar::CreateLayoutMenu() 
{
	auto& layoutMenu = CreateWidget<MenuList>("Layout");
	layoutMenu.CreateWidget<MenuItem>("Reset").ClickedEvent += EDITOR_BIND(ResetLayout);
}

void LittleEngine::Editor::Panels::MenuBar::CreateHelpMenu()
{
    auto& helpMenu = CreateWidget<MenuList>("Help");
    helpMenu.CreateWidget<MenuItem>("GitHub").ClickedEvent += [] {LittleEngine::Utils::SystemCalls::OpenURL("https://github.com/adriengivry/Overload"); };
    helpMenu.CreateWidget<MenuItem>("Tutorials").ClickedEvent += [] {LittleEngine::Utils::SystemCalls::OpenURL("https://github.com/adriengivry/Overload/wiki/Tutorials"); };
    helpMenu.CreateWidget<MenuItem>("Scripting API").ClickedEvent += [] {LittleEngine::Utils::SystemCalls::OpenURL("https://github.com/adriengivry/Overload/wiki/Scripting-API"); };
    helpMenu.CreateWidget<Visual::Separator>();
    helpMenu.CreateWidget<MenuItem>("Bug Report").ClickedEvent += [] {LittleEngine::Utils::SystemCalls::OpenURL("https://github.com/adriengivry/Overload/issues/new?assignees=&labels=Bug&template=bug_report.md&title="); };
    helpMenu.CreateWidget<MenuItem>("Feature Request").ClickedEvent += [] {LittleEngine::Utils::SystemCalls::OpenURL("https://github.com/adriengivry/Overload/issues/new?assignees=&labels=Feature&template=feature_request.md&title="); };
    helpMenu.CreateWidget<Visual::Separator>();
    helpMenu.CreateWidget<Texts::Text>("Version: 1.3.0");
}

void LittleEngine::Editor::Panels::MenuBar::RegisterPanel(const std::string& p_name, LittleEngine::UI::Panels::PanelWindow& p_panel)
{
	auto menuList = PorjectSettingName == p_name? m_editMenu : m_windowMenu;
	MenuItem& menuItem = menuList->CreateWidget<MenuItem>(p_name, "", true, true);
	menuItem.ValueChangedEvent += std::bind(&LittleEngine::UI::Panels::PanelWindow::SetOpened, &p_panel, std::placeholders::_1);
	m_panels.emplace(p_name, std::make_pair(std::ref(p_panel), std::ref(menuItem)));
}

void LittleEngine::Editor::Panels::MenuBar::UpdateToggleableItems()
{
	for (auto&[name, panel] : m_panels)
		panel.second.get().checked = panel.first.get().IsOpened();
}

void LittleEngine::Editor::Panels::MenuBar::OpenEveryWindows(bool p_state)
{
	for (auto&[name, panel] : m_panels)
		panel.first.get().SetOpened(p_state);
}
