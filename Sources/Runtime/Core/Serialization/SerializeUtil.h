#pragma once

#include "string"

#include "ISerializer.h"

namespace LittleEngine
{
	class SerializeUtil
	{
		typedef LittleEngine::Rendering::Resources::Model Model;
		typedef LittleEngine::Rendering::Resources::Texture Texture;
		typedef LittleEngine::Rendering::Resources::Shader Shader;
		typedef LittleEngine::Resources::Material Material;
		typedef LittleEngine::Audio::Resources::Sound Sound;
		typedef LittleEngine::Color Color;
	private:
		static LittleEngine::ISerializer* m_serializer;
	public:
		static void SetSerializer(ISerializer* p_serializer){m_serializer = p_serializer;}
		static void SerializeBoolean(const std::string& p_name, bool p_value){m_serializer->SerializeBoolean(p_name,p_value);}
		static void SerializeString(const std::string& p_name, const std::string& p_value){m_serializer->SerializeString(p_name,p_value);}
		static void SerializeFloat(const std::string& p_name, float p_value){m_serializer->SerializeFloat(p_name,p_value);}
		static void SerializeDouble(const std::string& p_name, double p_value){m_serializer->SerializeDouble(p_name,p_value);}
		static void SerializeInt(const std::string& p_name, int p_value){m_serializer->SerializeInt(p_name,p_value);}
		static void SerializeUint(const std::string& p_name, unsigned p_value){m_serializer->SerializeUint(p_name,p_value);}
		static void SerializeInt64(const std::string& p_name, int64_t p_value){m_serializer->SerializeInt64(p_name,p_value);}
		static void SerializeVec2(const std::string& p_name, const LittleEngine::FVector2& p_value){m_serializer->SerializeVec2(p_name,p_value);}
		static void SerializeVec3(const std::string& p_name, const LittleEngine::FVector3& p_value){m_serializer->SerializeVec3(p_name,p_value);}
		static void SerializeVec4(const std::string& p_name, const FVector4& p_value){m_serializer->SerializeVec4(p_name,p_value);}
		static void SerializeQuat(const std::string& p_name, const LittleEngine::FQuaternion& p_value){m_serializer->SerializeQuat(p_name,p_value);}
		static void SerializeColor(const std::string& p_name, const Color& p_value){m_serializer->SerializeColor(p_name,p_value);}
		static void SerializeModel(const std::string& p_name, Model* p_value){m_serializer->SerializeModel(p_name,p_value);}
		static void SerializeTexture(const std::string& p_name, Texture* p_value){m_serializer->SerializeTexture(p_name,p_value);}
		static void SerializeShader(const std::string& p_name, Shader* p_value){m_serializer->SerializeShader(p_name,p_value);}
		static void SerializeMaterial(const std::string& p_name, Material* p_value){m_serializer->SerializeMaterial(p_name,p_value);}
		static void SerializeSound(const std::string& p_name, Sound* p_value){m_serializer->SerializeSound(p_name,p_value);}

		static void DeserializeBoolean(const std::string& p_name, bool& p_out){m_serializer->DeserializeBoolean(p_name,p_out);}
		static void DeserializeString(const std::string& p_name, std::string& p_out){m_serializer->DeserializeString(p_name,p_out);}
		static void DeserializeFloat(const std::string& p_name, float& p_out){m_serializer->DeserializeFloat(p_name,p_out);}
		static void DeserializeDouble(const std::string& p_name, double& p_out){m_serializer->DeserializeDouble(p_name,p_out);}
		static void DeserializeInt(const std::string& p_name, int& p_out){m_serializer->DeserializeInt(p_name,p_out);}
		static void DeserializeUint(const std::string& p_name, unsigned& p_out){m_serializer->DeserializeUint(p_name,p_out);}
		static void DeserializeInt64(const std::string& p_name, int64_t& p_out){m_serializer->DeserializeInt64(p_name,p_out);}
		static void DeserializeUInt64(const std::string& p_name, uint64_t& p_out){m_serializer->DeserializeUInt64(p_name,p_out);}
		
		static void DeserializeVec2(const std::string& p_name, FVector2& p_out){m_serializer->DeserializeVec2(p_name,p_out);}
		static void DeserializeVec3(const std::string& p_name, FVector3& p_out){m_serializer->DeserializeVec3(p_name,p_out);}
		static void DeserializeVec4(const std::string& p_name, FVector4& p_out){m_serializer->DeserializeVec4(p_name,p_out);}
		static void DeserializeQuat(const std::string& p_name, FQuaternion& p_out){m_serializer->DeserializeQuat(p_name,p_out);}
		static void DeserializeColor(const std::string& p_name, Color& p_out){m_serializer->DeserializeColor(p_name,p_out);}
		static void DeserializeModel(const std::string& p_name, Model*& p_out){m_serializer->DeserializeModel(p_name,p_out);}
		static void DeserializeTexture(const std::string& p_name, Texture*& p_out){m_serializer->DeserializeTexture(p_name,p_out);}
		static void DeserializeShader(const std::string& p_name, Shader*& p_out){m_serializer->DeserializeShader(p_name,p_out);}
		static void DeserializeMaterial(const std::string& p_name, Material *& p_out){m_serializer->DeserializeMaterial(p_name,p_out);}
		static void DeserializeSound(const std::string& p_name, Sound*& p_out){m_serializer->DeserializeSound(p_name,p_out);}

		static bool 		DeserializeBoolean(const std::string& p_name){ return m_serializer->DeserializeBoolean(p_name);}
		static std::string 	DeserializeString(const std::string& p_name){ return m_serializer->DeserializeString(p_name);}
		static float 		DeserializeFloat(const std::string& p_name){ return m_serializer->DeserializeFloat(p_name);}
		static double 		DeserializeDouble(const std::string& p_name){ return m_serializer->DeserializeDouble(p_name);}
		static int 			DeserializeInt(const std::string& p_name){ return m_serializer->DeserializeInt(p_name);}
		static unsigned 	DeserializeUint(const std::string& p_name){ return m_serializer->DeserializeUint(p_name);}
		static int64_t 		DeserializeInt64(const std::string& p_name){ return m_serializer->DeserializeInt64(p_name);}
		static FVector2 	DeserializeVec2(const std::string& p_name){ return m_serializer->DeserializeVec2(p_name);}
		static FVector3 	DeserializeVec3(const std::string& p_name){ return m_serializer->DeserializeVec3(p_name);}
		static FVector4 	DeserializeVec4(const std::string& p_name){ return m_serializer->DeserializeVec4(p_name);}
		static FQuaternion 	DeserializeQuat(const std::string& p_name){ return m_serializer->DeserializeQuat(p_name);}
		static Color 		DeserializeColor(const std::string& p_name){ return m_serializer->DeserializeColor(p_name);}
		static Model* 		DeserializeModel(const std::string& p_name){ return m_serializer->DeserializeModel(p_name);}
		static Texture* 	DeserializeTexture(const std::string& p_name){ return m_serializer->DeserializeTexture(p_name);}
		static Shader* 		DeserializeShader(const std::string& p_name){ return m_serializer->DeserializeShader(p_name);}
		static Material* 	DeserializeMaterial(const std::string& p_name){ return m_serializer->DeserializeMaterial(p_name);}
		static Sound* 		DeserializeSound(const std::string& p_name){ return m_serializer->DeserializeSound(p_name);}

	};
}
