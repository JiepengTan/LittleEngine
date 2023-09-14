/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#define STB_IMAGE_IMPLEMENTATION

#include "GL/glew.h"
#include "stb_image/stb_image.h"
#include "Resource/Loader/TextureLoader.h"

void* LittleEngine::Resources::TextureLoader::LoadRawData(const std::string& p_filepath)
{
	
	int textureWidth;
	int textureHeight;
	int bitsPerPixel;
	unsigned char* dataBuffer = stbi_load(p_filepath.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
	return dataBuffer;
}

LittleEngine::Texture* LittleEngine::Resources::TextureLoader::Create(const std::string& p_filepath, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap ,bool p_flip)
{
	GLuint textureID;
	int textureWidth;
	int textureHeight;
	int bitsPerPixel;
	glGenTextures(1, &textureID);

	stbi_set_flip_vertically_on_load(p_flip);
	unsigned char* dataBuffer = stbi_load(p_filepath.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

	if (dataBuffer)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataBuffer);

		if (p_generateMipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		return new Texture(p_filepath, textureID, textureWidth, textureHeight, bitsPerPixel, p_firstFilter, p_secondFilter, p_generateMipmap);
	}
	else
	{
		stbi_image_free(dataBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		return nullptr;
	}
}

LittleEngine::Texture* LittleEngine::Resources::TextureLoader::CreateColor(uint32_t p_data, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &p_data);

	if (p_generateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture("", textureID, 1, 1, 32, p_firstFilter, p_secondFilter, p_generateMipmap);
}
LittleEngine::Texture* LittleEngine::Resources::TextureLoader::CreateFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height,
	LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter,
	LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap
	,const std::string& p_name )
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);

	if (p_generateMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(p_firstFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(p_secondFilter));

	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture("", textureID, p_width, p_height, 32, p_firstFilter, p_secondFilter, p_generateMipmap);
}

void LittleEngine::Resources::TextureLoader::Reload(Texture& p_texture, const std::string& p_filePath, LittleEngine::Rendering::Settings::ETextureFilteringMode p_firstFilter, LittleEngine::Rendering::Settings::ETextureFilteringMode p_secondFilter, bool p_generateMipmap)
{
	Texture* newTexture = Create(p_filePath, p_firstFilter, p_secondFilter, p_generateMipmap);

	if (newTexture)
	{
		glDeleteTextures(1, &p_texture.id);

		*const_cast<uint32_t*>(&p_texture.id) = newTexture->id;
		*const_cast<uint32_t*>(&p_texture.width) = newTexture->width;
		*const_cast<uint32_t*>(&p_texture.height) = newTexture->height;
		*const_cast<uint32_t*>(&p_texture.bitsPerPixel) = newTexture->bitsPerPixel;
		*const_cast<Rendering::Settings::ETextureFilteringMode*>(&p_texture.firstFilter) = newTexture->firstFilter;
		*const_cast<Rendering::Settings::ETextureFilteringMode*>(&p_texture.secondFilter) = newTexture->secondFilter;
		*const_cast<bool*>(&p_texture.isMimapped) = newTexture->isMimapped;
		delete newTexture;
	}
}

bool LittleEngine::Resources::TextureLoader::Destroy(Texture*& p_textureInstance)
{
	if (p_textureInstance)
	{
		glDeleteTextures(1, &p_textureInstance->id);
		delete p_textureInstance;
		p_textureInstance = nullptr;
		return true;
	}

	return false;
}
