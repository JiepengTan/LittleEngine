/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Analytics/Hardware/RAMInfo.h"

void LittleEngine::Analytics::Hardware::RAMInfo::Update()
{
	m_statex.dwLength = sizeof(m_statex);
	GlobalMemoryStatusEx(&m_statex);
}

float LittleEngine::Analytics::Hardware::RAMInfo::GetUsedRAM()
{
	return GetMaxRAM() - m_statex.ullAvailPhys / 1048576.0f;
}

float LittleEngine::Analytics::Hardware::RAMInfo::GetFreeRAM()
{
	return m_statex.ullAvailPhys / 1048576.0f;
}

float LittleEngine::Analytics::Hardware::RAMInfo::GetMaxRAM()
{
	return m_statex.ullTotalPhys / 1048576.0f;
}
