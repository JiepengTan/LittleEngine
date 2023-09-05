/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


namespace LittleEngine::UI::Plugins
{
	/**
	* Interface to any plugin of LittleEngine::UI.
	* A plugin is basically a behaviour that you can plug to a widget
	*/
	class IPlugin
	{
	public:
		/**
		* Execute the plugin behaviour
		*/
		virtual void Execute() = 0;

		/* Feel free to store any data you want here */
		void* userData = nullptr;
	};
}
