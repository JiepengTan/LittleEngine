/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"

#include "ISerializer.h"
#include "Core/Base/Color.h"

#include "Core/Maths/FVector2.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FVector4.h"
#include "Core/Maths/FQuaternion.h"
#include "tinyxml2/tinyxml2.h"



/* Forward declaration */
namespace LittleEngine::Rendering::Resources
{
	class Model;
	class Texture;
	class Shader;
}

namespace LittleEngine::Audio::Resources
{
	class Sound;
}

namespace LittleEngine::Resources
{
	class Material;
}

namespace LittleEngine
{

	class XmlSerializerXX : ISerializer
	{
		tinyxml2::XMLDocument p_doc;
		tinyxml2::XMLNode* p_node;
		typedef LittleEngine::Rendering::Resources::Model Model;
		typedef LittleEngine::Rendering::Resources::Texture Texture;
		typedef LittleEngine::Rendering::Resources::Shader Shader;
		typedef LittleEngine::Resources::Material Material;
		typedef LittleEngine::Audio::Resources::Sound Sound;
		typedef LittleEngine::Color Color;
	public:
		XmlSerializerXX() = default;
		virtual ~XmlSerializerXX()= default;
		
		#pragma region SERIALIZATION_HELPERS
		virtual void SerializeBoolean(const std::string& p_name, bool p_value) override;
		virtual void SerializeString(const std::string& p_name, const std::string& p_value) override;
		virtual void SerializeFloat(const std::string& p_name, float p_value) override;
		virtual void SerializeDouble(const std::string& p_name, double p_value) override;
		virtual void SerializeInt(const std::string& p_name, int p_value) override;
		virtual void SerializeUint(const std::string& p_name, unsigned p_value) override;
		virtual void SerializeInt64(const std::string& p_name, int64_t p_value) override;
		virtual void SerializeVec2(const std::string& p_name, const FVector2& p_value) override;
		virtual void SerializeVec3(const std::string& p_name, const FVector3& p_value) override;
		virtual void SerializeVec4(const std::string& p_name, const FVector4& p_value) override;
		virtual void SerializeQuat(const std::string& p_name, const FQuaternion& p_value) override;
		virtual void SerializeColor(const std::string& p_name, const Color& p_value) override;
		virtual void SerializeModel(const std::string& p_name, Model* p_value) override;
		virtual void SerializeTexture(const std::string& p_name, Texture* p_value) override;
		virtual void SerializeShader(const std::string& p_name, Shader* p_value) override;
		virtual void SerializeMaterial(const std::string& p_name, Material* p_value) override;
		virtual void SerializeSound(const std::string& p_name, Sound* p_value) override;
		#pragma endregion

		#pragma region DESERIALIZATION_HELPERS
		virtual void DeserializeBoolean(const std::string& p_name, bool& p_out) override;
		virtual void DeserializeString(const std::string& p_name, std::string& p_out) override;
		virtual void DeserializeFloat(const std::string& p_name, float& p_out) override;
		virtual void DeserializeDouble(const std::string& p_name, double& p_out) override;
		virtual void DeserializeInt(const std::string& p_name, int& p_out) override;
		virtual void DeserializeUint(const std::string& p_name, unsigned& p_out) override;
		virtual void DeserializeInt64(const std::string& p_name, int64_t& p_out) override;
		virtual void DeserializeUInt64(const std::string& p_name, uint64_t& p_out) override;
		
		virtual void DeserializeVec2(const std::string& p_name, FVector2& p_out) override;
		virtual void DeserializeVec3(const std::string& p_name, FVector3& p_out) override;
		virtual void DeserializeVec4(const std::string& p_name, FVector4& p_out) override;
		virtual void DeserializeQuat(const std::string& p_name, FQuaternion& p_out) override;
		virtual void DeserializeColor(const std::string& p_name, Color& p_out) override;
		virtual void DeserializeModel(const std::string& p_name, Model*& p_out) override;
		virtual void DeserializeTexture(const std::string& p_name, Texture*& p_out) override;
		virtual void DeserializeShader(const std::string& p_name, Shader*& p_out) override;
		virtual void DeserializeMaterial(const std::string& p_name, Material *& p_out) override;
		virtual void DeserializeSound(const std::string& p_name, Sound*& p_out) override;

		virtual bool DeserializeBoolean(const std::string& p_name) override;
		virtual std::string DeserializeString(const std::string& p_name) override;
		virtual float DeserializeFloat(const std::string& p_name) override;
		virtual double DeserializeDouble(const std::string& p_name) override;
		virtual int DeserializeInt(const std::string& p_name) override;
		virtual unsigned DeserializeUint(const std::string& p_name) override;
		virtual int64_t DeserializeInt64(const std::string& p_name) override;
		virtual FVector2 DeserializeVec2(const std::string& p_name) override;
		virtual FVector3 DeserializeVec3(const std::string& p_name) override;
		virtual FVector4 DeserializeVec4(const std::string& p_name) override;
		virtual FQuaternion DeserializeQuat(const std::string& p_name) override;
		virtual Color DeserializeColor(const std::string& p_name) override;
		virtual Model* DeserializeModel(const std::string& p_name) override;
		virtual Texture* DeserializeTexture(const std::string& p_name) override;
		virtual Shader* DeserializeShader(const std::string& p_name) override;
		virtual Material* DeserializeMaterial(const std::string& p_name) override;
		virtual Sound* DeserializeSound(const std::string& p_name) override;
		#pragma endregion
	};
}
