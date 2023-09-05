/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Resources/Loaders/ModelLoader.h"

#include "Modules/Rendering/Resources/Model.h"

LittleEngine::Rendering::Resources::Parsers::AssimpParser LittleEngine::Rendering::Resources::Loaders::ModelLoader::__ASSIMP;



LittleEngine::Rendering::Resources::Model* LittleEngine::Rendering::Resources::Loaders::ModelLoader::Create(const std::string& p_filepath,
                                                                                    LittleEngine::Rendering::Resources::Parsers::EModelParserFlags p_parserFlags)
{
	Model* result = new Model(p_filepath);
	
	if (__ASSIMP.LoadModel(result,p_filepath, result->m_meshes, result->m_materialNames, p_parserFlags))
	{
		result->ComputeBoundingSphere();
		return result;
	}
	
	delete result;

	return nullptr;
}



void LittleEngine::Rendering::Resources::Loaders::ModelLoader::Reload(Model& p_model, const std::string& p_filePath, Parsers::EModelParserFlags p_parserFlags)
{
	Model* newModel = Create(p_filePath, p_parserFlags);

	if (newModel)
	{
		p_model.m_meshes = newModel->m_meshes;
		p_model.m_materialNames = newModel->m_materialNames;
        p_model.m_boundingSphere = newModel->m_boundingSphere;
		newModel->m_meshes.clear();
		delete newModel;
	}
}

bool LittleEngine::Rendering::Resources::Loaders::ModelLoader::Destroy(Model*& p_modelInstance)
{
	if (p_modelInstance)
	{
		delete p_modelInstance;
		p_modelInstance = nullptr;

		return true;
	}

	return false;
}