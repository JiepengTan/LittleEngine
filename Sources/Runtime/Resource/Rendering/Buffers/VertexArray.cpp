/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Rendering/Buffers/VertexArray.h"

OvRendering::Buffers::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

OvRendering::Buffers::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void OvRendering::Buffers::VertexArray::Bind()
{
	//std::cout<< " Bind m_bufferID=  "<<m_bufferID<< std::endl;
	glBindVertexArray(m_bufferID);
}

void OvRendering::Buffers::VertexArray::Unbind()
{
	//std::cout<< " unbind m_bufferID=  "<<m_bufferID<< std::endl;
	glBindVertexArray(0);
}

GLint OvRendering::Buffers::VertexArray::GetID()
{
	return m_bufferID;
}
