/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <filesystem>

#include "Core/Tools/Utils/String.h"

#include "Modules/Rendering/Utils/Defines.h"

#include "../Editor/Core/ProjectHub.h"
#include "../Editor/Core/EditorApp.h"

#undef APIENTRY
#include "Windows.h"
#include "Core/Base/Application.h"

FORCE_DEDICATED_GPU

/**
* When Overload is launched from a project file, we should consider the executable path as
* the current working directory
* @param p_executablePath
*/
void UpdateWorkingDirectory(const std::string& p_executablePath)
{
	if (!IsDebuggerPresent())
	{
		std::filesystem::current_path(LittleEngine::Utils::PathParser::GetContainingFolder(p_executablePath));
	}
}

int main(int argc, char** argv);
static void TryRun(const std::string& projectPath, const std::string& projectName);

#ifndef _DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	main(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
	UpdateWorkingDirectory(argv[0]);

	bool ready = false;
	std::string projectPath;
	std::string projectName;

	{
		LittleEditor::Core::ProjectHub hub;

		if (argc < 2)
		{
			// No project file given as argument ==> Open the ProjectHub
			std::tie(ready, projectPath, projectName) = hub.Run();
		}
		else
		{
			// Project file given as argument ==> Open the project
			std::string projectFile = argv[1];

			if (LittleEngine::Utils::PathParser::GetExtension(projectFile) == "ovproject")
			{
				ready = true;
				projectPath = LittleEngine::Utils::PathParser::GetContainingFolder(projectFile);
				projectName = LittleEngine::Utils::PathParser::GetElementName(projectFile);
				LittleEngine::Utils::String::Replace(projectName, ".ovproject", "");
			}

			hub.RegisterProject(projectPath);
		}
	}

	LittleEngine::Application::isPlaying = false;
	LittleEngine::Application::isEditorMode = true;
	if (ready)
		TryRun(projectPath, projectName);

	return EXIT_SUCCESS;
}

static void TryRun(const std::string& projectPath, const std::string& projectName)
{
	auto errorEvent =
		[](LittleEngine::Windowing::Context::EDeviceError, std::string errMsg)
		{
			errMsg = "Overload requires OpenGL 4.3 or newer.\r\n" + errMsg;
			MessageBox(0, errMsg.c_str(), "Overload", MB_OK | MB_ICONSTOP);
		};

	std::unique_ptr<LittleEditor::Core::EditorApp> app;

	try
	{
		auto listenerId = LittleEngine::Windowing::Context::Device::ErrorEvent += errorEvent;
		app = std::make_unique<LittleEditor::Core::EditorApp>(projectPath, projectName);
		LittleEngine::Windowing::Context::Device::ErrorEvent -= listenerId;
	}
	catch (...) {}
	if (app)
		app->Run();
}
