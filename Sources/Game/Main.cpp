/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Utils/Defines.h"

#include "Core/Base/GlobalState.h"
#include "../Game/Core/Application.h"

FORCE_DEDICATED_GPU

#ifdef _DEBUG
int main()
#else
#undef APIENTRY
#include "Windows.h"
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	OvCore::GlobalState::IsPlaying = true;
	OvCore::GlobalState::IsEditorMode = false;
	OvGame::Core::Application app;
	app.Run();

	return EXIT_SUCCESS;
}
