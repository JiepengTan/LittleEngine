/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Utils/Defines.h"

#include "Core/Base/Application.h"
#include "../Game/Core/GameApp.h"
#include "Resource/ResInclude.h"

FORCE_DEDICATED_GPU

#ifdef _DEBUG
int main()
#else
#undef APIENTRY
#include "Windows.h"
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	LittleEngine::Application::isPlaying = true;
	LittleEngine::Application::isEditorMode = false;
	LittleGame::Core::GameApp app;
	app.Run();

	return EXIT_SUCCESS;
}
