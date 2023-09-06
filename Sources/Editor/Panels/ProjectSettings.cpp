/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Panels/ProjectSettings.h"
#include "../Editor/Core/EditorActions.h"

#include "Modules/Rendering/Resources/Loaders/MaterialLoader.h"
#include "Modules/Utils/GUIDrawer.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/UI/Widgets/Layout/GroupCollapsable.h"
#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Widgets/Buttons/Button.h"

using namespace LittleEngine::UI::Panels;
using namespace LittleEngine::UI::Widgets;
using namespace LittleEngine::Helpers;

LittleEngine::Editor::Panels::ProjectSettings::ProjectSettings(const std::string & p_title, bool p_opened, const LittleEngine::UI::Settings::PanelWindowSettings & p_windowSettings) :
	PanelWindow(p_title, p_opened, p_windowSettings),
	m_projectFile(EDITOR_CONTEXT(projectSettings))
{
	auto& saveButton = CreateWidget<Buttons::Button>("Apply");
	saveButton.idleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	saveButton.ClickedEvent += [this]
	{
		EDITOR_CONTEXT(ApplyProjectSettings());
		m_projectFile.Rewrite();
	};

	saveButton.lineBreak = false;

	auto& resetButton = CreateWidget<Buttons::Button>("Reset");
	resetButton.idleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	resetButton.ClickedEvent += [this]
	{
		EDITOR_CONTEXT(ResetProjectSettings());
	};

	CreateWidget<LittleEngine::UI::Widgets::Visual::Separator>();

	{
		/* Physics settings */
		auto& root = CreateWidget<Layout::GroupCollapsable>("Physics");
		auto& columns = root.CreateWidget<Layout::Columns<2>>();
		columns.widths[0] = 125;

		GUIDrawer::DrawScalar<float>(columns, "Gravity", GenerateGatherer<float>("gravity"), GenerateProvider<float>("gravity"), 0.1f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);
	}

	{
		/* Build settings */
		auto& generationRoot = CreateWidget<Layout::GroupCollapsable>("Build");
		auto& columns = generationRoot.CreateWidget<Layout::Columns<2>>();
		columns.widths[0] = 125;

		GUIDrawer::DrawBoolean(columns, "Development build", GenerateGatherer<bool>("dev_build"), GenerateProvider<bool>("dev_build"));
	}

	{
		/* Windowing settings */
		auto& windowingRoot = CreateWidget<Layout::GroupCollapsable>("Windowing");
		auto& columns = windowingRoot.CreateWidget<Layout::Columns<2>>();
		columns.widths[0] = 125;

		GUIDrawer::DrawScalar<int>(columns, "Resolution X", GenerateGatherer<int>("x_resolution"), GenerateProvider<int>("x_resolution"), 1, 0, 10000);
		GUIDrawer::DrawScalar<int>(columns, "Resolution Y", GenerateGatherer<int>("y_resolution"), GenerateProvider<int>("y_resolution"), 1, 0, 10000);
		GUIDrawer::DrawBoolean(columns, "Fullscreen", GenerateGatherer<bool>("fullscreen"), GenerateProvider<bool>("fullscreen"));
		GUIDrawer::DrawString(columns, "Executable name", GenerateGatherer<std::string>("executable_name"), GenerateProvider<std::string>("executable_name"));
	}

	{
		/* Rendering settings */
		auto& renderingRoot = CreateWidget<Layout::GroupCollapsable>("Rendering");
		auto& columns = renderingRoot.CreateWidget<Layout::Columns<2>>();
		columns.widths[0] = 125;

		GUIDrawer::DrawBoolean(columns, "Vertical Sync.", GenerateGatherer<bool>("vsync"), GenerateProvider<bool>("vsync"));
		GUIDrawer::DrawBoolean(columns, "Multi-sampling", GenerateGatherer<bool>("multisampling"), GenerateProvider<bool>("multisampling"));
		GUIDrawer::DrawScalar<int>(columns, "Samples", GenerateGatherer<int>("samples"), GenerateProvider<int>("samples"), 1, 2, 16);
		GUIDrawer::DrawScalar<int>(columns, "OpenGL Major", GenerateGatherer<int>("opengl_major"), GenerateProvider<int>("opengl_major"), 1, 3, 4);
		GUIDrawer::DrawScalar<int>(columns, "OpenGL Minor", GenerateGatherer<int>("opengl_minor"), GenerateProvider<int>("opengl_minor"), 1, 0, 6);
	}

	{
		/* Scene Management settings */
		auto& gameRoot = CreateWidget<Layout::GroupCollapsable>("Scene Management");
		auto& columns = gameRoot.CreateWidget<Layout::Columns<2>>();
		columns.widths[0] = 125;

		GUIDrawer::DrawDDString(columns, "Start scene", GenerateGatherer<std::string>("start_scene"), GenerateProvider<std::string>("start_scene"), "File");
	}
}
