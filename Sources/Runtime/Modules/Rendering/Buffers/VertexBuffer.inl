/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "iostream"
#include "GL/glew.h"

#include "Modules/Rendering/Buffers/VertexBuffer.h"

namespace LittleEngine::Rendering::Buffers
{
	template <class T>
	inline VertexBuffer<T>::VertexBuffer(void* p_data,size_t p_elementCount,size_t p_elemTypeSize)
	{
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, p_elementCount *p_elemTypeSize, p_data, GL_STATIC_DRAW);
		//std::cout<<"GL_ARRAY_BUFFER m_bufferID ="<<m_bufferID<< " VertexBuffer ctor  ptr= "<<(long)p_data<<" size =" << p_elementCount *p_elemTypeSize<< std::endl;
	}
	
	template <class T>
	inline VertexBuffer<T>::VertexBuffer(T* p_data, size_t p_elementCount)
	{
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, p_elementCount * sizeof(T), p_data, GL_STATIC_DRAW);
	}

	template<class T>
	inline VertexBuffer<T>::VertexBuffer(std::vector<T>& p_data) : VertexBuffer(p_data.data(), p_data.Size())
	{
	}
	
	template <class T>
	inline void VertexBuffer<T>::Rebind(void* p_data,size_t p_elementCount,size_t p_elemTypeSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, p_elementCount *p_elemTypeSize, p_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	template<class T>
	inline VertexBuffer<T>::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_bufferID);
	}

	template <class T>
	inline void VertexBuffer<T>::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		//std::cout<<"Bind GL_ARRAY_BUFFER m_bufferID ="<<m_bufferID<< std::endl;
	}

	template <class T>
	inline void VertexBuffer<T>::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <class T>
	inline uint32_t VertexBuffer<T>::GetID()
	{
		return m_bufferID;
	}
}
