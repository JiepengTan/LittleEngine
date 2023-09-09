/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/Manager/TextureManager.h"
#include "Resource/Manager/ModelManager.h"
#include "Resource/Manager/ShaderManager.h"

namespace LittleEngine::Editor::Core
{
	/**
	* Handle the creation and storage of editor specific resources
	*/
	class EditorResources
	{
	public:
		/**
		* Constructor
		* @param p_editorAssetsPath
		*/
		EditorResources(const std::string& p_editorAssetsPath);

		/**
		* Destructor
		*/
		~EditorResources();

		/**
		* Returns the file icon identified by the given string or nullptr on fail
		* @param p_filename
		*/
		LittleEngine::Texture* GetFileIcon(const std::string& p_filename);

		/**
		* Returns the texture identified by the given string or nullptr on fail
		* @param p_id
		*/
		LittleEngine::Texture* GetTexture(const std::string& p_id);

		/**
		* Returns the model identified by the given string or nullptr on fail
		* @param p_id
		*/
		LittleEngine::Model* GetModel(const std::string& p_id);

		/**
		* Returns the shader identified by the given string or nullptr on fail
		* @param p_id
		*/
		LittleEngine::Shader* GetShader(const std::string& p_id);

	private:
		std::unordered_map<std::string, LittleEngine::Texture*> m_textures;
		std::unordered_map<std::string, LittleEngine::Model*> m_models;
		std::unordered_map<std::string, LittleEngine::Shader*> m_shaders;
	};
}
