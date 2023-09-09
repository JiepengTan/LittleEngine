/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Resources/Loaders/MaterialLoader.h"

LittleEngine::Resources::Material * LittleEngine::Resources::Loaders::MaterialLoader::Create(const std::string & p_path)
{
	return nullptr;
}

void LittleEngine::Resources::Loaders::MaterialLoader::Reload(Material& p_material, const std::string& p_path)
{
}

void LittleEngine::Resources::Loaders::MaterialLoader::Save(Material& p_material, const std::string& p_path)
{

}

bool LittleEngine::Resources::Loaders::MaterialLoader::Destroy(Material *& p_material)
{
	return true;
}
