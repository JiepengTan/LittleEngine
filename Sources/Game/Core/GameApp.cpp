/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Time/Clock.h"

#include "../Game/Core/GameApp.h"

LittleGame::Core::GameApp::GameApp() :
	m_game(m_context)
{

}

LittleGame::Core::GameApp::~GameApp()
{
}

void LittleGame::Core::GameApp::Run()
{
	LittleEngine::Time::Clock clock;

	while (IsRunning())
	{
		m_game.PreUpdate();
		m_game.Update(clock.GetDeltaTime());
		m_game.PostUpdate();

		clock.Update();
	}
}

bool LittleGame::Core::GameApp::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
