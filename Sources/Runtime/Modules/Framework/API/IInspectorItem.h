/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/UI/Internal/WidgetContainer.h"

#include "Modules/Framework/API/ISerializable.h"
#include "Modules/Utils/GUIUtil.h"

namespace LittleEngine::API
{
	/**
	* IInspectorItem is an interface to any class that is serializable and drawable on inspector
	*/
	class IInspectorItem : public ISerializable
	{
	public:
		/**
		* Called when the inspector needs to create the UI elements for the given IInspectorItem
		* @param p_root
		*/
		virtual void OnInspector() = 0;
	};
}
