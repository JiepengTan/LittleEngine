/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "GL/glew.h"

#include "Modules/Rendering/Buffers/IndexBuffer.h"

#include "iostream"

LittleEngine::Rendering::Buffers::IndexBuffer::IndexBuffer(unsigned int* p_data, size_t p_elements)
{
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_elements * sizeof(unsigned int), p_data, GL_STATIC_DRAW);
	//std::cout<< " GL_ELEMENT_ARRAY_BUFFER   "<<m_bufferID <<"  ptr= " << p_data <<"  count= " << (int)p_elements * sizeof(unsigned int) <<std::endl;
}

LittleEngine::Rendering::Buffers::IndexBuffer::IndexBuffer(std::vector<uint32_t>& p_data) : IndexBuffer(p_data.data(), p_data.size())
{
}

LittleEngine::Rendering::Buffers::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void LittleEngine::Rendering::Buffers::IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void LittleEngine::Rendering::Buffers::IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t LittleEngine::Rendering::Buffers::IndexBuffer::GetID()
{
	return m_bufferID;
}
