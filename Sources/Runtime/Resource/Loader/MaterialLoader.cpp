/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Loader/MaterialLoader.h"

#include "Core/Tools/Filesystem/FileUtil.h"
#include "_Generated/Serializer/Material.serializer.gen.h"

LittleEngine::Material * LittleEngine::Resources::MaterialLoader::Create(const std::string & p_path)
{
	Material* ret_instance= new Material;
	auto jsonStr = FileUtil::ReadAllText(p_path);
	std::string error;
	auto&& jsonContext = Json::parse(jsonStr, error);
	if(jsonContext == nullptr)
	{
		LOG_ERROR("Load material failed Path =" + p_path + "  error= " + error);
	}
	LittleEngine::JsonSerializer::Read(jsonContext, *ret_instance);
	return ret_instance;
}

void LittleEngine::Resources::MaterialLoader::Reload(Material& p_material, const std::string& p_path)
{
	auto jsonStr = FileUtil::ReadAllText(p_path);
	std::string error;
	auto&& jsonContext = Json::parse(jsonStr, error);
	LittleEngine::JsonSerializer::Read(jsonContext, p_material);
}

void LittleEngine::Resources::MaterialLoader::Save(Material& p_material, const std::string& p_path)
{
	auto jsonContext = p_material.ToJson();
	auto&& jsonStr = jsonContext.dump();
	FileUtil::WriteAllText(p_path,jsonStr);
}

bool LittleEngine::Resources::MaterialLoader::Destroy(Material *& p_material)
{
	if (p_material)
	{
		delete p_material;
		p_material = nullptr;
	}
	return true;	
}
