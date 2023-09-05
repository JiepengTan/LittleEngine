/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Modules/Canvas.h"
#include "Modules/Framework/ECS/Components/CCamera.h"

#include "../Game/Core/Context.h"
#include "../Game/Core/GameRenderer.h"

#include "../Game/Utils/FPSCounter.h"

#ifdef _DEBUG
#include "../Game/Debug/DriverInfo.h"
#include "../Game/Debug/GameProfiler.h"
#include "../Game/Debug/FrameInfo.h"
#endif

namespace LittleGame::Core
{
	/**
	* Handle the game logic
	*/
	class Game
	{
	public:
		/**
		* Create the game
		* @param p_context
		*/
		Game(Context& p_context);

		/**
		* Destroy the game
		*/
		~Game();

		/**
		* Pre-update of the game logic
		*/
		void PreUpdate();

		/**
		* Update the game logic
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Post-update of the game logic
		*/
		void PostUpdate();

	private:
		float m_elapsed = 0.0f;

		LittleGame::Core::Context& m_context;
		LittleEngine::UI::Modules::Canvas m_canvas;

		LittleGame::Core::GameRenderer m_gameRenderer;

		/* Debug elements */
		LittleGame::Utils::FPSCounter	m_fpsCounter;

		#ifdef _DEBUG
		LittleGame::Debug::DriverInfo	m_driverInfo;
		LittleGame::Debug::GameProfiler m_gameProfiler;
		LittleGame::Debug::FrameInfo	m_frameInfo;
		#endif

		#ifdef _DEBUG
		bool m_showDebugInformation = true;
		#else
		bool m_showDebugInformation = false;
		#endif
	};
}
