/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Game/Core/Context.h"
#include "../Game/Core/Game.h"

namespace LittleGame::Core
{
	/**
	* Entry point of LittleGame
	*/
	class GameApp
	{
	public:
		/**
		* Constructor
		*/
		GameApp();

		/**
		* Destructor
		*/
		~GameApp();

		/**
		* Run the app
		*/
		void Run();

		/**
		* Returns true if the app is running
		*/
		bool IsRunning() const;

	private:
		Context m_context;
		Game m_game;
	};
}
