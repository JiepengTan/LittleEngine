#pragma once

#include "string"
#include "Core/Base/Color.h"
#include "Core/Maths/FVector2.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FVector4.h"
#include "Core/Maths/FQuaternion.h"

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
namespace LittleEngine{
	class IBaseSerializer
	{
		
	};
	class ISerializer
	{
		typedef LittleEngine::Rendering::Resources::Model Model;
		typedef LittleEngine::Rendering::Resources::Texture Texture;
		typedef LittleEngine::Rendering::Resources::Shader Shader;
		typedef LittleEngine::Resources::Material Material;
		typedef LittleEngine::Audio::Resources::Sound Sound;
		typedef LittleEngine::Color Color;
	public:
		ISerializer() = default;
		virtual ~ISerializer()= default;
		virtual void Clear(){}
		#pragma region SERIALIZATION_HELPERS
		virtual void SerializeBoolean(const std::string& p_name, bool p_value){}
		virtual void SerializeString(const std::string& p_name, const std::string& p_value){}
		virtual void SerializeFloat(const std::string& p_name, float p_value){}
		virtual void SerializeDouble(const std::string& p_name, double p_value){}
		virtual void SerializeInt(const std::string& p_name, int p_value){}
		virtual void SerializeUint(const std::string& p_name, unsigned p_value){}
		virtual void SerializeInt64(const std::string& p_name, int64_t p_value){}
		virtual void SerializeVec2(const std::string& p_name, const FVector2& p_value){}
		virtual void SerializeVec3(const std::string& p_name, const FVector3& p_value){}
		virtual void SerializeVec4(const std::string& p_name, const FVector4& p_value){}
		virtual void SerializeQuat(const std::string& p_name, const FQuaternion& p_value){}
		virtual void SerializeColor(const std::string& p_name, const Color& p_value){}
		virtual void SerializeModel(const std::string& p_name, Model* p_value){}
		virtual void SerializeTexture(const std::string& p_name, Texture* p_value){}
		virtual void SerializeShader(const std::string& p_name, Shader* p_value){}
		virtual void SerializeMaterial(const std::string& p_name, Material* p_value){}
		virtual void SerializeSound(const std::string& p_name, Sound* p_value){}
		#pragma endregion

		#pragma region DESERIALIZATION_HELPERS
		virtual void DeserializeBoolean(const std::string& p_name, bool& p_out){}
		virtual void DeserializeString(const std::string& p_name, std::string& p_out){}
		virtual void DeserializeFloat(const std::string& p_name, float& p_out){}
		virtual void DeserializeDouble(const std::string& p_name, double& p_out){}
		virtual void DeserializeInt(const std::string& p_name, int& p_out){}
		virtual void DeserializeUint(const std::string& p_name, unsigned& p_out){}
		virtual void DeserializeInt64(const std::string& p_name, int64_t& p_out){}
		virtual void DeserializeUInt64(const std::string& p_name, uint64_t& p_out){}
		
		virtual void DeserializeVec2(const std::string& p_name, FVector2& p_out){}
		virtual void DeserializeVec3(const std::string& p_name, FVector3& p_out){}
		virtual void DeserializeVec4(const std::string& p_name, FVector4& p_out){}
		virtual void DeserializeQuat(const std::string& p_name, FQuaternion& p_out){}
		virtual void DeserializeColor(const std::string& p_name, Color& p_out){}
		virtual void DeserializeModel(const std::string& p_name, Model*& p_out){}
		virtual void DeserializeTexture(const std::string& p_name, Texture*& p_out){}
		virtual void DeserializeShader(const std::string& p_name, Shader*& p_out){}
		virtual void DeserializeMaterial(const std::string& p_name, Material *& p_out){}
		virtual void DeserializeSound(const std::string& p_name, Sound*& p_out){}

		virtual bool DeserializeBoolean(const std::string& p_name){return false;}
		virtual std::string DeserializeString(const std::string& p_name){return "";}
		virtual float DeserializeFloat(const std::string& p_name){return false;}
		virtual double DeserializeDouble(const std::string& p_name){return false;}
		virtual int DeserializeInt(const std::string& p_name){return false;}
		virtual unsigned DeserializeUint(const std::string& p_name){return false;}
		virtual int64_t DeserializeInt64(const std::string& p_name){return false;}
		virtual FVector2 DeserializeVec2(const std::string& p_name){return false;}
		virtual FVector3 DeserializeVec3(const std::string& p_name){return false;}
		virtual FVector4 DeserializeVec4(const std::string& p_name){return false;}
		virtual FQuaternion DeserializeQuat(const std::string& p_name){return false;}
		virtual Color DeserializeColor(const std::string& p_name){return Color::Black;}
		virtual Model* DeserializeModel(const std::string& p_name){return nullptr;}
		virtual Texture* DeserializeTexture(const std::string& p_name){return nullptr;}
		virtual Shader* DeserializeShader(const std::string& p_name){return nullptr;}
		virtual Material* DeserializeMaterial(const std::string& p_name){return nullptr;}
		virtual Sound* DeserializeSound(const std::string& p_name){return nullptr;}
		#pragma endregion
	};

}
