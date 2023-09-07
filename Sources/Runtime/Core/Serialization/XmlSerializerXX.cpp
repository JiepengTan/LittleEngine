/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "XmlSerializerXX.h"

#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/SoundManager.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Core/Serialization/XmlSerializerXX.h"

void LittleEngine::XmlSerializerXX::SerializeBoolean( const std::string & p_name, bool p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeString( const std::string & p_name, const std::string & p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value.c_str());
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeFloat( const std::string & p_name, float p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeDouble( const std::string & p_name, double p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeInt( const std::string & p_name, int p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeUint( const std::string & p_name, unsigned p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeInt64( const std::string & p_name, int64_t p_value)
{
	tinyxml2::XMLElement* element = p_doc.NewElement(p_name.c_str());
	element->SetText(p_value);
	p_node->InsertEndChild(element);
}

void LittleEngine::XmlSerializerXX::SerializeVec2( const std::string & p_name, const LittleEngine::FVector2 & p_value)
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

void LittleEngine::XmlSerializerXX::SerializeVec3( const std::string & p_name, const LittleEngine::FVector3 & p_value)
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

void LittleEngine::XmlSerializerXX::SerializeVec4( const std::string & p_name, const LittleEngine::FVector4 & p_value)
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

void LittleEngine::XmlSerializerXX::SerializeQuat( const std::string & p_name, const LittleEngine::FQuaternion & p_value)
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

void LittleEngine::XmlSerializerXX::SerializeColor( const std::string & p_name, const LittleEngine::Color & p_value)
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

void LittleEngine::XmlSerializerXX::SerializeModel( const std::string & p_name, LittleEngine::Rendering::Resources::Model * p_value)
{
	SerializeString(p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::XmlSerializerXX::SerializeTexture( const std::string & p_name, LittleEngine::Rendering::Resources::Texture * p_value)
{
	SerializeString(p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::XmlSerializerXX::SerializeShader( const std::string & p_name, LittleEngine::Rendering::Resources::Shader * p_value)
{
	SerializeString(p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::XmlSerializerXX::DeserializeBoolean( const std::string & p_name, bool & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryBoolText(&p_out);
}

void LittleEngine::XmlSerializerXX::SerializeMaterial( const std::string & p_name, LittleEngine::Resources::Material * p_value)
{
	SerializeString(p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::XmlSerializerXX::SerializeSound(const std::string& p_name, LittleEngine::Audio::Resources::Sound* p_value)
{
	SerializeString(p_name.c_str(), p_value ? p_value->path : "?");
}

void LittleEngine::XmlSerializerXX::DeserializeString( const std::string & p_name, std::string & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
	{
		const char* result = element->GetText();
		p_out = result ? result : "";
	}
}

void LittleEngine::XmlSerializerXX::DeserializeFloat( const std::string & p_name, float & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryFloatText(&p_out);
}

void LittleEngine::XmlSerializerXX::DeserializeDouble( const std::string & p_name, double & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryDoubleText(&p_out);
}

void LittleEngine::XmlSerializerXX::DeserializeInt( const std::string & p_name, int & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryIntText(&p_out);
}

void LittleEngine::XmlSerializerXX::DeserializeUint( const std::string & p_name, unsigned & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryUnsignedText(&p_out);
}

void LittleEngine::XmlSerializerXX::DeserializeInt64( const std::string & p_name, int64_t & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
		element->QueryInt64Text(&p_out);
}
void LittleEngine::XmlSerializerXX::DeserializeUInt64( const std::string & p_name, uint64_t & p_out)
{
	if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
	{
		int64_t val;
		element->QueryInt64Text(&val);
		p_out = val;
	}
}
void LittleEngine::XmlSerializerXX::DeserializeVec2( const std::string & p_name, LittleEngine::FVector2 & p_out)
{
	if (auto node = p_node->FirstChildElement(p_name.c_str()); node)
	{
		if (auto element = node->FirstChildElement("x"); element)
			element->QueryFloatText(&p_out.x);

		if (auto element = node->FirstChildElement("y"); element)
			element->QueryFloatText(&p_out.y);
	}
}

void LittleEngine::XmlSerializerXX::DeserializeVec3( const std::string & p_name, LittleEngine::FVector3 & p_out)
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

void LittleEngine::XmlSerializerXX::DeserializeVec4( const std::string & p_name, LittleEngine::FVector4 & p_out)
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

void LittleEngine::XmlSerializerXX::DeserializeQuat( const std::string & p_name, LittleEngine::FQuaternion & p_out)
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

void LittleEngine::XmlSerializerXX::DeserializeColor( const std::string & p_name, LittleEngine::Color & p_out)
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

void LittleEngine::XmlSerializerXX::DeserializeModel( const std::string & p_name, LittleEngine::Rendering::Resources::Model *& p_out)
{
	if (std::string path = DeserializeString(p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::ModelManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::XmlSerializerXX::DeserializeTexture( const std::string & p_name, LittleEngine::Rendering::Resources::Texture *& p_out)
{
	if (std::string path = DeserializeString(p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::TextureManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::XmlSerializerXX::DeserializeShader( const std::string & p_name, LittleEngine::Rendering::Resources::Shader *& p_out)
{
	if (std::string path = DeserializeString(p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::ShaderManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::XmlSerializerXX::DeserializeMaterial( const std::string & p_name, LittleEngine::Resources::Material *& p_out)
{
	if (std::string path = DeserializeString(p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::MaterialManager>().GetResource(path);
	else
		p_out = nullptr;
}

void LittleEngine::XmlSerializerXX::DeserializeSound(const std::string& p_name, LittleEngine::Audio::Resources::Sound*& p_out)
{
	if (std::string path = DeserializeString(p_name.c_str()); path != "?" && path != "")
		p_out = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::SoundManager>().GetResource(path);
	else
		p_out = nullptr;
}

bool LittleEngine::XmlSerializerXX::DeserializeBoolean( const std::string & p_name)
{
	bool result;
	DeserializeBoolean(p_name, result);
	return result;
}

std::string LittleEngine::XmlSerializerXX::DeserializeString( const std::string & p_name)
{
	std::string result;
	DeserializeString(p_name, result);
	return result;
}

float LittleEngine::XmlSerializerXX::DeserializeFloat( const std::string & p_name)
{
	float result;
	DeserializeFloat(p_name, result);
	return result;
}

double LittleEngine::XmlSerializerXX::DeserializeDouble( const std::string & p_name)
{
	double result;
	DeserializeDouble(p_name, result);
	return result;
}

int LittleEngine::XmlSerializerXX::DeserializeInt( const std::string & p_name)
{
	int result;
	DeserializeInt(p_name, result);
	return result;
}

unsigned LittleEngine::XmlSerializerXX::DeserializeUint( const std::string & p_name)
{
	unsigned result;
	DeserializeUint(p_name, result);
	return result;
}

int64_t LittleEngine::XmlSerializerXX::DeserializeInt64( const std::string & p_name)
{
	int64_t result;
	DeserializeInt64(p_name, result);
	return result;
}

LittleEngine::FVector2 LittleEngine::XmlSerializerXX::DeserializeVec2( const std::string & p_name)
{
	LittleEngine::FVector2 result;
	DeserializeVec2(p_name, result);
	return result;
}

LittleEngine::FVector3 LittleEngine::XmlSerializerXX::DeserializeVec3( const std::string & p_name)
{
	LittleEngine::FVector3 result;
	DeserializeVec3(p_name, result);
	return result;
}

LittleEngine::FVector4 LittleEngine::XmlSerializerXX::DeserializeVec4( const std::string & p_name)
{
	LittleEngine::FVector4 result;
	DeserializeVec4(p_name, result);
	return result;
}

LittleEngine::FQuaternion LittleEngine::XmlSerializerXX::DeserializeQuat( const std::string & p_name)
{
	LittleEngine::FQuaternion result;
	DeserializeQuat(p_name, result);
	return result;
}

LittleEngine::Color LittleEngine::XmlSerializerXX::DeserializeColor( const std::string & p_name)
{
	LittleEngine::Color result;
	DeserializeColor(p_name, result);
	return result;
}

LittleEngine::Rendering::Resources::Model* LittleEngine::XmlSerializerXX::DeserializeModel( const std::string & p_name)
{
	LittleEngine::Rendering::Resources::Model* result;
	DeserializeModel(p_name, result);
	return result;
}

LittleEngine::Rendering::Resources::Texture* LittleEngine::XmlSerializerXX::DeserializeTexture( const std::string & p_name)
{
	LittleEngine::Rendering::Resources::Texture* result;
	DeserializeTexture(p_name, result);
	return result;
}

LittleEngine::Rendering::Resources::Shader* LittleEngine::XmlSerializerXX::DeserializeShader( const std::string & p_name)
{
	LittleEngine::Rendering::Resources::Shader* result;
	DeserializeShader(p_name, result);
	return result;
}

LittleEngine::Resources::Material * LittleEngine::XmlSerializerXX::DeserializeMaterial( const std::string & p_name)
{
	LittleEngine::Resources::Material* result;
	DeserializeMaterial(p_name, result);
	return result;
}

LittleEngine::Audio::Resources::Sound* LittleEngine::XmlSerializerXX::DeserializeSound(const std::string& p_name)
{
	LittleEngine::Audio::Resources::Sound* result;
	DeserializeSound(p_name, result);
	return result;
}
