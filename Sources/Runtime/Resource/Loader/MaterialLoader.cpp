/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Loader/MaterialLoader.h"

LittleEngine::Material * LittleEngine::Resources::MaterialLoader::Create(const std::string & p_path)
{
	return nullptr;
}

void LittleEngine::Resources::MaterialLoader::Reload(Material& p_material, const std::string& p_path)
{
}

void LittleEngine::Resources::MaterialLoader::Save(Material& p_material, const std::string& p_path)
{

}

bool LittleEngine::Resources::MaterialLoader::Destroy(Material *& p_material)
{
	return true;
}
