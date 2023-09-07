/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Resources/Loaders/MaterialLoader.h"

LittleEngine::Resources::Material * LittleEngine::Resources::Loaders::MaterialLoader::Create(const std::string & p_path)
{
	// TODO tanjp
#ifdef false
	tinyxml2::XMLDocument doc;
	doc.LoadFile(p_path.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();

		Material* material = new Material();

		material->OnDeserialize();

		return material;
	}
	else
	{
		return nullptr;
	}
#endif
	
		return nullptr;
}

void LittleEngine::Resources::Loaders::MaterialLoader::Reload(Material& p_material, const std::string& p_path)
{
#ifdef false
	tinyxml2::XMLDocument doc;
	doc.LoadFile(p_path.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();

		p_material.OnDeserialize(doc, root);
	}
#endif
}

void LittleEngine::Resources::Loaders::MaterialLoader::Save(Material& p_material, const std::string& p_path)
{
#ifdef false
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
	doc.InsertFirstChild(node);
	p_material.OnSerialize(doc, node);

	if (doc.SaveFile(p_path.c_str()) == tinyxml2::XML_SUCCESS)
		LOG_INFO("[MATERIAL] \"" + p_path + "\": Saved");
	else
		LOG_ERROR("[MATERIAL] \"" + p_path + "\": Failed to save");
#endif
}

bool LittleEngine::Resources::Loaders::MaterialLoader::Destroy(Material *& p_material)
{
#ifdef false
	if (p_material)
	{
		delete p_material;
		p_material = nullptr;

		return true;
	}
#endif
	return false;
}
