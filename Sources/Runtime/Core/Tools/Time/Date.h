/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"


namespace LittleEngine::Time
{
	/*
	* Date system to get the current date in a string format
	*/
	class Date
	{
	public:
		/**
		* Default constructor
		*/
		Date() = delete;

		/*
		* Return the current date in a string format
		*/
		static std::string GetDateAsString();
		static std::string GetTimeAsString();
	};
}
