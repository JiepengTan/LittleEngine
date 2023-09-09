/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Debug/Assertion.h"

#include "assert.h"
#include "iostream"

void OvDebug::Assertion::Assert(bool p_condition, const std::string& p_message)
{
	if(!p_condition)
		std::cout<<"Asset Failed !! "<< p_message <<std::endl;
	assert(p_condition && p_message.c_str());
}
