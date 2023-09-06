/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"

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
		LittleEngine::Rendering::Resources::Texture* GetFileIcon(const std::string& p_filename);

		/**
		* Returns the texture identified by the given string or nullptr on fail
		* @param p_id
		*/
		LittleEngine::Rendering::Resources::Texture* GetTexture(const std::string& p_id);

		/**
		* Returns the model identified by the given string or nullptr on fail
		* @param p_id
		*/
		LittleEngine::Rendering::Resources::Model* GetModel(const std::string& p_id);

		/**
		* Returns the shader identified by the given string or nullptr on fail
		* @param p_id
		*/
		LittleEngine::Rendering::Resources::Shader* GetShader(const std::string& p_id);

	private:
		std::unordered_map<std::string, LittleEngine::Rendering::Resources::Texture*> m_textures;
		std::unordered_map<std::string, LittleEngine::Rendering::Resources::Model*> m_models;
		std::unordered_map<std::string, LittleEngine::Rendering::Resources::Shader*> m_shaders;
	};
}
