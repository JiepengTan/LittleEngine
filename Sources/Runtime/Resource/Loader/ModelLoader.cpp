/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Loader/ModelLoader.h"

#include "Resource/Asset/Model.h"

LittleEngine::Resources::AssimpParser LittleEngine::Resources::ModelLoader::__ASSIMP;



LittleEngine::Model* LittleEngine::Resources::ModelLoader::Create(const std::string& p_filepath,
                                                                                    LittleEngine::Resources::EModelParserFlags p_parserFlags)
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



void LittleEngine::Resources::ModelLoader::Reload(Model& p_model, const std::string& p_filePath, EModelParserFlags p_parserFlags)
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

bool LittleEngine::Resources::ModelLoader::Destroy(Model*& p_modelInstance)
{
	if (p_modelInstance)
	{
		delete p_modelInstance;
		p_modelInstance = nullptr;

		return true;
	}

	return false;
}
