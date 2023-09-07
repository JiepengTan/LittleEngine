/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Resources/Material.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"

#include "Modules/Rendering/Buffers/UniformBuffer.h"
#include "Modules/Rendering/Resources/Texture.h"

void LittleEngine::Resources::Material::SetShader(LittleEngine::Rendering::Resources::Shader* p_shader)
{
	m_shader = p_shader;
	if (m_shader)
	{
		LittleEngine::Rendering::Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
	else
	{
		m_uniformsData.clear();
	}
}

void LittleEngine::Resources::Material::FillUniform()
{
	m_uniformsData.clear();
	for (const LittleEngine::Rendering::Resources::UniformInfo& element : m_shader->uniforms)
		m_uniformsData.emplace(element.name, element.defaultValue);
}

void LittleEngine::Resources::Material::Bind(LittleEngine::Rendering::Resources::Texture* p_emptyTexture)
{
	if (HasShader())
	{
		using namespace LittleEngine;
		using namespace LittleEngine::Rendering::Resources;

		m_shader->Bind();

		int textureSlot = 0;

		for (auto&[name, value] : m_uniformsData)
		{
			auto uniformData = m_shader->GetUniformInfo(name);

			if (uniformData)
			{
				switch (uniformData->type)
				{
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_BOOL:			if (value.type() == typeid(bool))		m_shader->SetUniformInt(name, std::any_cast<bool>(value));			break;
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_INT:			if (value.type() == typeid(int))		m_shader->SetUniformInt(name, std::any_cast<int>(value));			break;
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT:		if (value.type() == typeid(float))		m_shader->SetUniformFloat(name, std::any_cast<float>(value));		break;
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT_VEC2:	if (value.type() == typeid(FVector2))	m_shader->SetUniformVec2(name, std::any_cast<FVector2>(value));		break;
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT_VEC3:	if (value.type() == typeid(FVector3))	m_shader->SetUniformVec3(name, std::any_cast<FVector3>(value));		break;
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT_VEC4:	if (value.type() == typeid(FVector4))	m_shader->SetUniformVec4(name, std::any_cast<FVector4>(value));		break;
				case LittleEngine::Rendering::Resources::UniformType::UNIFORM_SAMPLER_2D:
					{
						if (value.type() == typeid(Texture*))
						{
							if (auto tex = std::any_cast<Texture*>(value); tex)
							{
								tex->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
							else if (p_emptyTexture)
							{
								p_emptyTexture->Bind(textureSlot);
								m_shader->SetUniformInt(uniformData->name, textureSlot++);
							}
						}
					}
				}
			}
		}
	}
}

void LittleEngine::Resources::Material::UnBind()
{
	if (HasShader())
		m_shader->Unbind();
}

LittleEngine::Rendering::Resources::Shader*& LittleEngine::Resources::Material::GetShader()
{
	return m_shader;
}

bool LittleEngine::Resources::Material::HasShader() const
{
	return m_shader;
}

void LittleEngine::Resources::Material::SetBlendable(bool p_transparent)
{
	m_blendable = p_transparent;
}

void LittleEngine::Resources::Material::SetBackfaceCulling(bool p_backfaceCulling)
{
	m_backfaceCulling = p_backfaceCulling;
}

void LittleEngine::Resources::Material::SetFrontfaceCulling(bool p_frontfaceCulling)
{
	m_frontfaceCulling = p_frontfaceCulling;
}

void LittleEngine::Resources::Material::SetDepthTest(bool p_depthTest)
{
	m_depthTest = p_depthTest;
}

void LittleEngine::Resources::Material::SetDepthWriting(bool p_depthWriting)
{
	m_depthWriting = p_depthWriting;
}

void LittleEngine::Resources::Material::SetColorWriting(bool p_colorWriting)
{
	m_colorWriting = p_colorWriting;
}

void LittleEngine::Resources::Material::SetGPUInstances(int p_instances)
{
	m_gpuInstances = p_instances;
}

bool LittleEngine::Resources::Material::IsBlendable() const
{
	return m_blendable;
}

bool LittleEngine::Resources::Material::HasBackfaceCulling() const
{
	return m_backfaceCulling;
}

bool LittleEngine::Resources::Material::HasFrontfaceCulling() const
{
	return m_frontfaceCulling;
}

bool LittleEngine::Resources::Material::HasDepthTest() const
{
	return m_depthTest;
}

bool LittleEngine::Resources::Material::HasDepthWriting() const
{
	return m_depthWriting;
}

bool LittleEngine::Resources::Material::HasColorWriting() const
{
	return m_colorWriting;
}

int LittleEngine::Resources::Material::GetGPUInstances() const
{
	return m_gpuInstances;
}

uint8_t LittleEngine::Resources::Material::GenerateStateMask() const
{
	uint8_t result = 0;

	if (m_depthWriting)								result |= 0b0000'0001;
	if (m_colorWriting)								result |= 0b0000'0010;
	if (m_blendable)								result |= 0b0000'0100;
	if (m_backfaceCulling || m_frontfaceCulling)	result |= 0b0000'1000;
	if (m_depthTest)								result |= 0b0001'0000;
	if (m_backfaceCulling)							result |= 0b0010'0000;
	if (m_frontfaceCulling)							result |= 0b0100'0000;

	return result;
}

std::map<std::string, std::any>& LittleEngine::Resources::Material::GetUniformsData()
{
	return m_uniformsData;
}

void LittleEngine::Resources::Material::OnSerialize(ISerializer p_serializer)
{
	// TODO tanjp 
#ifdef false
	using namespace LittleEngine::Rendering::Resources;
	using namespace LittleEngine;

	SerializeUtil::SerializeShader("shader", m_shader);

	tinyxml2::XMLNode* settingsNode = p_doc.NewElement("settings");
	p_node->InsertEndChild(settingsNode);

	SerializeUtil::SerializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
	SerializeUtil::SerializeBoolean(p_doc, settingsNode, "backface_culling", m_backfaceCulling);
	SerializeUtil::SerializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontfaceCulling);
	SerializeUtil::SerializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
	SerializeUtil::SerializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
	SerializeUtil::SerializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
	SerializeUtil::SerializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);

	// Create "Uniforms" (Every uniform will be attached to "Uniforms")
	tinyxml2::XMLNode* uniformsNode = p_doc.NewElement("uniforms");
	p_node->InsertEndChild(uniformsNode);

	for (const auto&[uniformName, uniformValue] : m_uniformsData)
	{
		tinyxml2::XMLNode* uniform = p_doc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform); // Instead of p_node, use uniformNode (To create)

		const LittleEngine::Rendering::Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

		SerializeUtil::SerializeString(p_doc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case UniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool)) SerializeUtil::SerializeInt(p_doc, uniform, "value", std::any_cast<bool>(uniformValue));
				break;

			case UniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int)) SerializeUtil::SerializeInt(p_doc, uniform, "value", std::any_cast<int>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float)) SerializeUtil::SerializeFloat(p_doc, uniform, "value", std::any_cast<float>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(FVector2)) SerializeUtil::SerializeVec2(p_doc, uniform, "value", std::any_cast<FVector2>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(FVector3)) SerializeUtil::SerializeVec3(p_doc, uniform, "value", std::any_cast<FVector3>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(FVector4)) SerializeUtil::SerializeVec4(p_doc, uniform, "value", std::any_cast<FVector4>(uniformValue));
				break;

			case UniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(Texture*)) SerializeUtil::SerializeTexture(p_doc, uniform, "value", std::any_cast<Texture*>(uniformValue));
				break;
			}
		}
	}
		*/
#endif
			
}

void LittleEngine::Resources::Material::OnDeserialize(ISerializer p_serializer)
{ 
#ifdef false
	tinyxml2::XMLNode* settingsNode = p_node->FirstChildElement("settings");
	
	if (settingsNode)
	{
		SerializeUtil::DeserializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
		SerializeUtil::DeserializeBoolean(p_doc, settingsNode, "backface_culling", m_backfaceCulling);
		SerializeUtil::DeserializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontfaceCulling);
		SerializeUtil::DeserializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
		SerializeUtil::DeserializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
		SerializeUtil::DeserializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
		SerializeUtil::DeserializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);
	}

	//We get the shader with Deserialize method
	LittleEngine::Rendering::Resources::Shader* deserializedShader = SerializeUtil::DeserializeShader("shader");

	//We verify that the shader is valid (Not null)
	if (deserializedShader)
	{
		/* If the shader is valid, we set it to the material (Modify m_shader pointer + Query + FillUniforms) */
		SetShader(deserializedShader);

		tinyxml2::XMLNode* uniformsNode = p_node->FirstChildElement("uniforms"); // Access to "Uniforms" (Every uniform will be attached to "Uniforms")		

		if (uniformsNode)
		{
			/* We iterate over every <uniform>...</uniform> */
			for (auto uniform = uniformsNode->FirstChildElement("uniform"); uniform; uniform = uniform->NextSiblingElement("uniform"))
			{
				/* Verify that the uniform node contains a "name" element */
				if (auto uniformNameElement = uniform->FirstChildElement("name"); uniformNameElement)
				{
					const std::string uniformName = uniformNameElement->GetText();

					/* We collect information about the uniform (The uniform is identified in the shader by its name) */
					const LittleEngine::Rendering::Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

					/* We verify that the uniform is existant is the current shader */
					if (uniformInfo)
					{
						/* Deserialization of the uniform value depending on the uniform type (Deserialization result to std::any) */
						switch (uniformInfo->type)
						{
						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_BOOL:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeBoolean(p_doc, uniform, "value");
							break;

						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_INT:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeInt(p_doc, uniform, "value");
							break;

						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeFloat(p_doc, uniform, "value");
							break;

						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT_VEC2:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeVec2(p_doc, uniform, "value");
							break;

						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT_VEC3:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeVec3(p_doc, uniform, "value");
							break;

						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_FLOAT_VEC4:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeVec4(p_doc, uniform, "value");
							break;

						case LittleEngine::Rendering::Resources::UniformType::UNIFORM_SAMPLER_2D:
							m_uniformsData[uniformInfo->name] = SerializeUtil::DeserializeTexture(p_doc, uniform, "value");
							break;
						}
					}
				}
			}
		}
			
	}
		*/
#endif
}
