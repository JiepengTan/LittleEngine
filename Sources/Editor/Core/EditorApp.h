/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "../Editor/Core/Context.h"
#include "../Editor/Core/Editor.h"

namespace LittleEngine::Editor::Core
{
	/**
	* Entry point of LittleEngine::Editor
	*/
	class EditorApp
	{
	public:
		/**
		* Constructor
		* @param p_projectPath
		* @param p_projectName
		*/
		EditorApp(const std::string& p_projectPath, const std::string& p_projectName);

		/**
		* Destructor
		*/
		~EditorApp();

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
		Editor m_editor;
	};
}
