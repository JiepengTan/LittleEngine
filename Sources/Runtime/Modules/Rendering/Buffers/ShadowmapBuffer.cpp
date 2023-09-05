/**
* @project: Overload
* @author: gamestan
* @licence: MIT
*/

#include <GL/glew.h>

#include "Modules/Rendering/Buffers/ShadowmapBuffer.h"

#include <memory>

#include "Core/CoreInclude.h"


OvRendering::Buffers::ShadowmapBuffer::ShadowmapBuffer(uint16_t p_width, uint16_t p_height)
{
	m_width = p_width;
	m_height = p_height;
	
	/* Generate OpenGL objects */
	glGenFramebuffers(1, &m_bufferID);
	glGenTextures(1, &m_renderTexture);
	glGenRenderbuffers(1, &m_depthStencilBuffer);

	OVLOG_INFO(
		"ShadowmapBuffer m_bufferID= " + std::to_string(m_bufferID)
		+ " m_renderTexture= " + std::to_string(m_renderTexture)
		+ " m_depthStencilBuffer= " + std::to_string(m_depthStencilBuffer) );
	/* Setup texture */
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_texturePtr = new Resources::Texture(m_renderTexture);
	/* Setup framebuffer */
	Resize(m_width, m_height);
}

OvRendering::Buffers::ShadowmapBuffer::~ShadowmapBuffer()
{
	/* Destroy OpenGL objects */
	glDeleteBuffers(1, &m_bufferID);
	glDeleteTextures(1, &m_renderTexture);
	glDeleteRenderbuffers(1, &m_depthStencilBuffer);
	delete m_texturePtr;
}

void OvRendering::Buffers::ShadowmapBuffer::Bind()
{
	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OvRendering::Buffers::ShadowmapBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OvRendering::Buffers::ShadowmapBuffer::Resize(uint16_t p_width, uint16_t p_height)
{
	m_width = p_width;
	m_height = p_height;
	/* Resize texture */
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Attach depth and stencil buffer to the framebuffer */
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_renderTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t OvRendering::Buffers::ShadowmapBuffer::GetID()
{
	return m_bufferID;
}

uint32_t OvRendering::Buffers::ShadowmapBuffer::GetTextureID()
{
	return m_renderTexture;
}

OvRendering::Resources::Texture* OvRendering::Buffers::ShadowmapBuffer::GetTexture()
{
	return m_texturePtr;
}

uint32_t OvRendering::Buffers::ShadowmapBuffer::GetRenderBufferID()
{
	return m_depthStencilBuffer;
}
