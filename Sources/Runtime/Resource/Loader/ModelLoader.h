/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"

#include "Resource/Parser/AssimpParser.h"
#include "Resource/Asset/Model.h"

namespace LittleEngine::Resources
{
	/**
	* Handle the Model creation and destruction
	*/
	class ModelLoader
	{
	public:
		/**
		* Disabled constructor
		*/
		ModelLoader() = delete;

		/**
		* Create a model
		* @param p_filepath
		* @param p_parserFlags
		*/
		static Model* Create(const std::string& p_filepath, EModelParserFlags p_parserFlags = EModelParserFlags::NONE);

		/**
		* Reload a model from file
		* @param p_model
		* @param p_filePath
		* @param p_parserFlags
		*/
		static void Reload(Model& p_model, const std::string& p_filePath, EModelParserFlags p_parserFlags = EModelParserFlags::NONE);

		/**
		* Disabled constructor
		* @param p_modelInstance
		*/
		static bool Destroy(Model*& p_modelInstance);

	private:
		static AssimpParser __ASSIMP;
	};
}
