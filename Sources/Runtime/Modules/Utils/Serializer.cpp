/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/SoundManager.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Utils/Serializer.h"

void LittleEngine::Serializer::SerializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, bool p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const std::string & p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value.c_str());
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, float p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, double p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, unsigned p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int64_t p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::Serializer::SerializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const LittleEngine::FVector2 & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);
}

void LittleEngine::Serializer::SerializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const LittleEngine::FVector3 & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = p_doc.NewElement("z");
	z->SetText(p_value.z);
	element->InsertEndChild(z);
}

void LittleEngine::Serializer::SerializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const LittleEngine::FVector4 & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = p_doc.NewElement("z");
	z->SetText(p_value.z);
	element->InsertEndChild(z);

	tinyxml2::XMLElement* w = p_doc.NewElement("w");
	w->SetText(p_value.w);
	element->InsertEndChild(w);
}

void LittleEngine::Serializer::SerializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const LittleEngine::FQuaternion & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* x = p_doc.NewElement("x");
	x->SetText(p_value.x);
	element->InsertEndChild(x);

	tinyxml2::XMLElement* y = p_doc.NewElement("y");
	y->SetText(p_value.y);
	element->InsertEndChild(y);

	tinyxml2::XMLElement* z = p_doc.NewElement("z");
	z->SetText(p_value.z);
	element->InsertEndChild(z);

	tinyxml2::XMLElement* w = p_doc.NewElement("w");
	w->SetText(p_value.w);
	element->InsertEndChild(w);
}

void LittleEngine::Serializer::SerializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, const LittleEngine::UI::Types::Color & p_value)
{
	tinyxml2::XMLNode* element = p_doc.NewElement(p_name.c_str());
	p_node->InsertEndChild(element);

	tinyxml2::XMLElement* r = p_doc.NewElement("r");
	r->SetText(p_value.r);
	element->InsertEndChild(r);

	tinyxml2::XMLElement* g = p_doc.NewElement("g");
	g->SetText(p_value.g);
	element->InsertEndChild(g);

	tinyxml2::XMLElement* b = p_doc.NewElement("b");
	b->SetText(p_value.b);
	element->InsertEndChild(b);

	tinyxml2::XMLElement* a = p_doc.NewElement("a");
	a->SetText(p_value.a);
	element->InsertEndChild(a);
}

void LittleEngine::Serializer::SerializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Rendering::Resources::Model * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::Serializer::SerializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Rendering::Resources::Texture * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::Serializer::SerializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Rendering::Resources::Shader * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::Serializer::DeserializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, bool & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryBoolText(&p_out);
}

void LittleEngine::Serializer::SerializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Resources::Material * p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::Serializer::SerializeSound(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, LittleEngine::Audio::Resources::Sound* p_value)
{
	SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::Serializer::DeserializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, std::string & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
	{
		const char* result = element->GetText();
		p_out = result ? result : "";
	}
}

void LittleEngine::Serializer::DeserializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, float & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryFloatText(&p_out);
}

void LittleEngine::Serializer::DeserializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, double & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryDoubleText(&p_out);
}

void LittleEngine::Serializer::DeserializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryIntText(&p_out);
}

void LittleEngine::Serializer::DeserializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, unsigned & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryUnsignedText(&p_out);
}

void LittleEngine::Serializer::DeserializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, int64_t & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryInt64Text(&p_out);
}
void LittleEngine::Serializer::DeserializeUInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, uint64_t & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
	{
		int64_t val;
		element->QueryInt64Text(&val);
		p_out = val;
	}
}
void LittleEngine::Serializer::DeserializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::FVector2 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);
	}
}

void LittleEngine::Serializer::DeserializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::FVector3 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&p_out.z);
	}
}

void LittleEngine::Serializer::DeserializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::FVector4 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&p_out.z);

		if (auto element = node->FirstChildElement("w"); element)
			element->QueryFloatText(&p_out.w);
	}
}

void LittleEngine::Serializer::DeserializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::FQuaternion & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);

		if (auto element = node->FirstChildElement("z"); element)
			element->QueryFloatText(&p_out.z);

		if (auto element = node->FirstChildElement("w"); element)
			element->QueryFloatText(&p_out.w);
	}
}

void LittleEngine::Serializer::DeserializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::UI::Types::Color & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("r"); element)
			element->QueryFloatText(&p_out.r);

		if (auto element = node->FirstChildElement("g"); element)
			element->QueryFloatText(&p_out.g);

		if (auto element = node->FirstChildElement("b"); element)
			element->QueryFloatText(&p_out.b);

		if (auto element = node->FirstChildElement("q"); element)
			element->QueryFloatText(&p_out.a);
	}
}

void LittleEngine::Serializer::DeserializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Rendering::Resources::Model *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::ModelManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::Serializer::DeserializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Rendering::Resources::Texture *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::TextureManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::Serializer::DeserializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Rendering::Resources::Shader *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::ShaderManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::Serializer::DeserializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name, LittleEngine::Resources::Material *& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::MaterialManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::Serializer::DeserializeSound(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, LittleEngine::Audio::Resources::Sound*& p_out)
{
	if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::SoundManager>().GetResource(path);
	else
		p_out = nullptr;
}

bool LittleEngine::Serializer::DeserializeBoolean(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	bool result;
	DeserializeBoolean(p_doc, p_node, p_name, result);
	return result;
}

std::string LittleEngine::Serializer::DeserializeString(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	std::string result;
	DeserializeString(p_doc, p_node, p_name, result);
	return result;
}

float LittleEngine::Serializer::DeserializeFloat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	float result;
	DeserializeFloat(p_doc, p_node, p_name, result);
	return result;
}

double LittleEngine::Serializer::DeserializeDouble(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	double result;
	DeserializeDouble(p_doc, p_node, p_name, result);
	return result;
}

int LittleEngine::Serializer::DeserializeInt(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	int result;
	DeserializeInt(p_doc, p_node, p_name, result);
	return result;
}

unsigned LittleEngine::Serializer::DeserializeUint(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	unsigned result;
	DeserializeUint(p_doc, p_node, p_name, result);
	return result;
}

int64_t LittleEngine::Serializer::DeserializeInt64(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	int64_t result;
	DeserializeInt64(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::FVector2 LittleEngine::Serializer::DeserializeVec2(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::FVector2 result;
	DeserializeVec2(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::FVector3 LittleEngine::Serializer::DeserializeVec3(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::FVector3 result;
	DeserializeVec3(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::FVector4 LittleEngine::Serializer::DeserializeVec4(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::FVector4 result;
	DeserializeVec4(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::FQuaternion LittleEngine::Serializer::DeserializeQuat(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::FQuaternion result;
	DeserializeQuat(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::UI::Types::Color LittleEngine::Serializer::DeserializeColor(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::UI::Types::Color result;
	DeserializeColor(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::Rendering::Resources::Model* LittleEngine::Serializer::DeserializeModel(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::Rendering::Resources::Model* result;
	DeserializeModel(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::Rendering::Resources::Texture* LittleEngine::Serializer::DeserializeTexture(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::Rendering::Resources::Texture* result;
	DeserializeTexture(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::Rendering::Resources::Shader* LittleEngine::Serializer::DeserializeShader(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::Rendering::Resources::Shader* result;
	DeserializeShader(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::Resources::Material * LittleEngine::Serializer::DeserializeMaterial(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node, const std::string & p_name)
{
	LittleEngine::Resources::Material* result;
	DeserializeMaterial(p_doc, p_node, p_name, result);
	return result;
}

LittleEngine::Audio::Resources::Sound* LittleEngine::Serializer::DeserializeSound(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
	LittleEngine::Audio::Resources::Sound* result;
	DeserializeSound(p_doc, p_node, p_name, result);
	return result;
}
