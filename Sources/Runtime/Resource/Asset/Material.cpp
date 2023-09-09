/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Asset/Material.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/TextureManager.h"

#include "Modules/Rendering/Buffers/UniformBuffer.h"
#include "Resource/Asset/Texture.h"

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
				case UniformType::UNIFORM_BOOL:			if (value.type() == typeid(bool))		m_shader->SetUniformInt(name, std::any_cast<bool>(value));			break;
				case UniformType::UNIFORM_INT:			if (value.type() == typeid(int))		m_shader->SetUniformInt(name, std::any_cast<int>(value));			break;
				case UniformType::UNIFORM_FLOAT:		if (value.type() == typeid(float))		m_shader->SetUniformFloat(name, std::any_cast<float>(value));		break;
				case UniformType::UNIFORM_FLOAT_VEC2:	if (value.type() == typeid(FVector2))	m_shader->SetUniformVec2(name, std::any_cast<FVector2>(value));		break;
				case UniformType::UNIFORM_FLOAT_VEC3:	if (value.type() == typeid(FVector3))	m_shader->SetUniformVec3(name, std::any_cast<FVector3>(value));		break;
				case UniformType::UNIFORM_FLOAT_VEC4:	if (value.type() == typeid(FVector4))	m_shader->SetUniformVec4(name, std::any_cast<FVector4>(value));		break;
				case UniformType::UNIFORM_SAMPLER_2D:
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
			
}

void LittleEngine::Resources::Material::OnDeserialize(ISerializer p_serializer)
{ 
	
}
