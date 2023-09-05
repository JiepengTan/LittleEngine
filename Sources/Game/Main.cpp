/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Utils/Defines.h"

#include "Core/Base/GlobalState.h"
#include "../Game/Core/GameApp.h"

FORCE_DEDICATED_GPU

#ifdef _DEBUG
int main()
#else
#undef APIENTRY
#include "Windows.h"
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	LittleEngine::GlobalState::IsPlaying = true;
	LittleEngine::GlobalState::IsEditorMode = false;
	LittleGame::Core::GameApp app;
	app.Run();

	return EXIT_SUCCESS;
}
