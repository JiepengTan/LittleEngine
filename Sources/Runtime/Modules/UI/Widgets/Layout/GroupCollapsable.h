/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>

#include "Core/Tools/Eventing/Event.h"

#include "Modules/UI/Widgets/Layout/Group.h"

namespace LittleEngine::UI::Widgets::Layout
{
	/**
	* Widget that can contains other widgets and is collapsable
	*/
	class GroupCollapsable : public Group
	{
	public:
		/**
		* Constructor
		* @param p_name
		*/
		GroupCollapsable(const std::string& p_name = "");

	protected:
		virtual void _Draw_Impl() override;

	public:
		std::string name;
		bool closable = false;
		bool opened = true;
		LittleEngine::Eventing::Event<> CloseEvent;
		LittleEngine::Eventing::Event<> OpenEvent;
	};
}
