/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "vector"

#include "Modules/Rendering/Context/Driver.h"

namespace LittleEngine::Rendering::Buffers
{
	/**
	* Wraps from OpenGL types enum
	*/
	enum class EType
	{
		BYTE			= 0x1400,
		UNISGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		DOUBLE			= 0x140A
	};
	/**
	* Wraps OpenGL VBO
	*/
	template <class T>
	class VertexBuffer
	{
	public:
		/**
		* Create the VBO using a pointer to the first element and a size (number of elements)
		* @param p_data
		* @parma p_elementCount
		*/
		VertexBuffer(T* p_data, size_t p_elementCount);
		
		VertexBuffer(void* p_data,size_t p_elementCount,size_t p_elemTypeSize);
		/**
		* Create the EBO using a vector
		* @param p_data
		*/
		VertexBuffer(std::vector<T>& p_data);
		
		void Rebind(void* p_data,size_t p_elementCount,size_t p_elemTypeSize);
		/**
		* Destructor
		*/
		~VertexBuffer();

		/**
		* Bind the buffer
		*/
		void Bind();

		
		/**
		* Bind the buffer
		*/
		void Unbind();

		/**
		* Returnd the ID of the VBO
		*/
		uint32_t GetID();

	private:
		uint32_t m_bufferID;
	};
}

#include "Modules/Rendering/Buffers/VertexBuffer.inl"
