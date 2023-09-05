/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>

#include "Core/Tools/Eventing/Event.h"

#include "Modules/UI/Widgets/DataWidget.h"

namespace LittleEngine::UI::Widgets::Selection
{
	/**
	* Widget that can display a list of values that the user can select
	*/
	class ComboBox : public DataWidget<int>
	{
	public:
		/**
		* Constructor
		* @param p_currentChoice
		*/
		ComboBox(int p_currentChoice = 0);

	protected:
		void _Draw_Impl() override;

	public:
		std::map<int, std::string> choices;
		int currentChoice;

	public:
		LittleEngine::Eventing::Event<int> ValueChangedEvent;
	};
}
