/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Buffers/ShaderStorageBuffer.h"
#include "UniformBuffer.h"

namespace LittleEngine::Rendering::Buffers
{
	template<typename T>
	inline void ShaderStorageBuffer::SendBlocks(T* p_data, size_t p_size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, p_size, p_data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}
