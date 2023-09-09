/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"



namespace OvDebug
{
	/**
	* Wrapper for C++ assert
	*/
	class Assertion
	{
	public:

		/**
		* Disabled constructor
		*/
		Assertion() = delete;

		/**
		* C++ assertion wrapped call
		* @param p_condition
		* @param p_message
		*/
		static void Assert(bool p_condition, const std::string& p_message = "");
	};
}
