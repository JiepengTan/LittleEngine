/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "stdint.h"
#include "string"

#include "Modules/Rendering/Settings/ETextureFilteringMode.h"



namespace LittleEngine::Rendering::Buffers { class ShadowmapBuffer; }
namespace LittleEngine
{
	namespace Resources { class TextureLoader; }

	/**
	* OpenGL texture wrapper
	*/
	class Texture
	{
		friend class Resources::TextureLoader;
		friend class LittleEngine::Rendering::Buffers::ShadowmapBuffer;
	public:
		/**
		* Bind the texture to the given slot
		* @param p_slot
		*/
		void Bind(uint32_t p_slot = 0) const;

		/**
		* Unbind the texture
		*/
		void Unbind() const;

	private:
		Texture(uint32_t p_id);
		Texture(const std::string p_path, uint32_t p_id, uint32_t p_width, uint32_t p_height, uint32_t p_bpp,
		        Rendering::Settings::ETextureFilteringMode p_firstFilter, Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap);
		~Texture() = default;

	public:
		const uint32_t id;
		const uint32_t width;
		const uint32_t height;
		const uint32_t bitsPerPixel;
		const Rendering::Settings::ETextureFilteringMode firstFilter;
		const Rendering::Settings::ETextureFilteringMode secondFilter;
		std::string path;
		const bool isMimapped;
	};
}
