/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Buffers/VertexArray.h"

LittleEngine::Rendering::Buffers::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

LittleEngine::Rendering::Buffers::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void LittleEngine::Rendering::Buffers::VertexArray::Bind()
{
	//std::cout<< " Bind m_bufferID=  "<<m_bufferID<< std::endl;
	glBindVertexArray(m_bufferID);
}

void LittleEngine::Rendering::Buffers::VertexArray::Unbind()
{
	//std::cout<< " unbind m_bufferID=  "<<m_bufferID<< std::endl;
	glBindVertexArray(0);
}

GLint LittleEngine::Rendering::Buffers::VertexArray::GetID()
{
	return m_bufferID;
}
