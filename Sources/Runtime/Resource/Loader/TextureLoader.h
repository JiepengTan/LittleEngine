/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"
#include "vector"

#include "Resource/Asset/Texture.h"


namespace LittleEngine::Resources
{
	/**
	* Handle the Texture creation and destruction
	*/
	class TextureLoader
	{
	public:
		/**
		* Disabled constructor
		*/
		TextureLoader() = delete;

		static void* LoadRawData(const std::string& p_filepath);
		/**
		* Create a texture from file
		* @param p_filePath
		* @param p_firstFilter
		* @param p_secondFilter
		* @param p_generateMipmap
		*/
		static Texture* Create(const std::string& p_filepath, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap, bool p_flip= true);

		/**
		* Create a texture from a single pixel color
		* @param p_data
		* @param p_firstFilder
		* @param p_secondFilter
		* @param p_generateMipmap
		*/
		static Texture* CreateColor(uint32_t p_data, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap);

		/**
		* Create a texture from memory
		* @param p_data
		* @param p_width
		* @param p_height
		* @param p_firstFilder
		* @param p_secondFilter
		* @param p_generateMipmap
		*/
		static Texture* CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap,const std::string& p_name = "");

		/**
		* Reload a texture from file
		* @param p_texture
		* @param p_filePath
		* @param p_firstFilter
		* @param p_secondFilter
		* @param p_generateMipmap
		*/
		static void Reload(Texture& p_texture, const std::string& p_filePath, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap);

		/**
		* Destroy a texture
		* @param p_textureInstance
		*/
		static bool Destroy(Texture*& p_textureInstance);
	};
}
