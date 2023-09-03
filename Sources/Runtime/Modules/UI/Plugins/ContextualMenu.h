/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Plugins/IPlugin.h"
#include "Modules/UI/Internal/WidgetContainer.h"
#include "Modules/UI/Widgets/Menu/MenuList.h"
#include "Modules/UI/Widgets/Menu/MenuItem.h"

namespace OvUI::Plugins
{
	/**
	* A simple plugin that will show a contextual menu on right click
	* You can add widgets to a contextual menu
	*/
	class ContextualMenu : public IPlugin, public Internal::WidgetContainer
	{
	public:
		/**
		* Execute the plugin
		*/
		void Execute();

		/**
		* Force close the contextual menu
		*/
		void Close();
	};
}
