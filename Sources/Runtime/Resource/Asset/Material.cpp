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


namespace LittleEngine
{
    void Material::SetShader(Shader* p_shader)
    {
        m_shader = p_shader;
        if (m_shader)
        {
            Rendering::Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
            FillUniform();
        }
        else
        {
            m_uniformsData.clear();
        }
    }

    void Material::FillUniform()
    {
        m_uniformsData.clear();
        for (const Resources::UniformInfo& element : m_shader->uniforms)
            m_uniformsData.emplace(element.name, element.defaultValue);
    }

    void Material::Bind(Texture* p_emptyTexture)
    {
        if (HasShader())
        {
            using namespace LittleEngine;
            using namespace Resources;

            m_shader->Bind();

            int textureSlot = 0;

            for (auto& [name, value] : m_uniformsData)
            {
                auto uniformData = m_shader->GetUniformInfo(name);

                if (uniformData)
                {
                    switch (uniformData->type)
                    {
                    case UniformType::UNIFORM_BOOL: if (value.type() == typeid(bool)) m_shader->SetUniformInt(
                            name, std::any_cast<bool>(value));
                        break;
                    case UniformType::UNIFORM_INT: if (value.type() == typeid(int)) m_shader->SetUniformInt(
                            name, std::any_cast<int>(value));
                        break;
                    case UniformType::UNIFORM_FLOAT: if (value.type() == typeid(float)) m_shader->SetUniformFloat(
                            name, std::any_cast<float>(value));
                        break;
                    case UniformType::UNIFORM_FLOAT_VEC2: if (value.type() == typeid(FVector2)) m_shader->
                            SetUniformVec2(name, std::any_cast<FVector2>(value));
                        break;
                    case UniformType::UNIFORM_FLOAT_VEC3: if (value.type() == typeid(FVector3)) m_shader->
                            SetUniformVec3(name, std::any_cast<FVector3>(value));
                        break;
                    case UniformType::UNIFORM_FLOAT_VEC4: if (value.type() == typeid(FVector4)) m_shader->
                            SetUniformVec4(name, std::any_cast<FVector4>(value));
                        break;
                    case UniformType::UNIFORM_SAMPLER_2D:
                        {
                            if (value.type() == typeid(LittleEngine::Texture*))
                            {
                                if (auto tex = std::any_cast<LittleEngine::Texture*>(value); tex)
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

    void Material::UnBind()
    {
        if (HasShader())
            m_shader->Unbind();
    }

    Shader*& Material::GetShader()
    {
        return m_shader;
    }

    bool Material::HasShader() const
    {
        return m_shader;
    }

    void Material::SetBlendable(bool p_transparent)
    {
        m_blendable = p_transparent;
    }

    void Material::SetBackfaceCulling(bool p_backfaceCulling)
    {
        m_backfaceCulling = p_backfaceCulling;
    }

    void Material::SetFrontfaceCulling(bool p_frontfaceCulling)
    {
        m_frontfaceCulling = p_frontfaceCulling;
    }

    void Material::SetDepthTest(bool p_depthTest)
    {
        m_depthTest = p_depthTest;
    }

    void Material::SetDepthWriting(bool p_depthWriting)
    {
        m_depthWriting = p_depthWriting;
    }

    void Material::SetColorWriting(bool p_colorWriting)
    {
        m_colorWriting = p_colorWriting;
    }

    void Material::SetGPUInstances(int p_instances)
    {
        m_gpuInstances = p_instances;
    }

    bool Material::IsBlendable() const
    {
        return m_blendable;
    }

    bool Material::HasBackfaceCulling() const
    {
        return m_backfaceCulling;
    }

    bool Material::HasFrontfaceCulling() const
    {
        return m_frontfaceCulling;
    }

    bool Material::HasDepthTest() const
    {
        return m_depthTest;
    }

    bool Material::HasDepthWriting() const
    {
        return m_depthWriting;
    }

    bool Material::HasColorWriting() const
    {
        return m_colorWriting;
    }

    int Material::GetGPUInstances() const
    {
        return m_gpuInstances;
    }

    uint8_t Material::GenerateStateMask() const
    {
        uint8_t result = 0;

        if (m_depthWriting) result |= 0b0000'0001;
        if (m_colorWriting) result |= 0b0000'0010;
        if (m_blendable) result |= 0b0000'0100;
        if (m_backfaceCulling || m_frontfaceCulling) result |= 0b0000'1000;
        if (m_depthTest) result |= 0b0001'0000;
        if (m_backfaceCulling) result |= 0b0010'0000;
        if (m_frontfaceCulling) result |= 0b0100'0000;

        return result;
    }

    std::map<std::string, std::any>& Material::GetUniformsData()
    {
        return m_uniformsData;
    }

    void Material::OnSerialize(ISerializer p_serializer)
    {
    }

    void Material::OnDeserialize(ISerializer p_serializer)
    {
    }
}
