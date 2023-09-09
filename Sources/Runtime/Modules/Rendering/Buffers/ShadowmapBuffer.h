/**
* @project: Overload
* @author: gamestan
* @licence: MIT
*/

#pragma once

#include "memory"

#include "Modules/Rendering/Context/Driver.h"
#include "Resource/Asset/Texture.h"

namespace LittleEngine::Rendering::Buffers
{
    /**
    * Wraps Shadowmap
    */
    class ShadowmapBuffer
    {
    public:
        /**
        * Create the framebuffer
        * @param p_width
        * @param p_height
        */
        ShadowmapBuffer(uint16_t p_width = 512, uint16_t p_height = 512);

        /**
        * Destructor
        */
        ~ShadowmapBuffer();

        /**
        * Bind the framebuffer
        */
        void Bind();

        /**
        * Unbind the framebuffer
        */
        void Unbind();

        /**
        * Defines a new size for the framebuffer
        * @param p_width
        * @param p_height
        */
        void Resize(uint16_t p_width, uint16_t p_height);

        /**
        * Returns the ID of the OpenGL framebuffer
        */
        uint32_t GetID();

        /**
        * Returns the ID of the OpenGL render texture
        */
        uint32_t GetTextureID();

        LittleEngine::Rendering::Resources::Texture* GetTexture();
        /**
        * Returns the ID of the OpenGL render buffer
        */
        uint32_t GetRenderBufferID();

    private:
        uint32_t m_bufferID = 0;
        uint32_t m_renderTexture = 0;
        uint32_t m_depthStencilBuffer = 0;
        uint16_t m_width = 0;
        uint16_t m_height = 0;
        Resources::Texture* m_texturePtr;
    };
}
