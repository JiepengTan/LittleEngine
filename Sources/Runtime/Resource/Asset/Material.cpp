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
        SetShader(p_shader->path);
    }
    void Material::SetShader(StringText p_path)
    {
        m_res.m_shader = ResourcesUtils::LoadShaderResPtr(p_path);
        if (m_res.m_shader)
        {
            Rendering::Buffers::UniformBuffer::BindBlockToShader(*m_res.m_shader, "EngineUBO");
            FillUniform();
        }
        else
        {
            m_res.m_uniformsData.clear();
        }
    }

    void Material::FillUniform()
    {
        m_res.m_uniformsData.clear();
        for (const Resources::UniformInfo& element : m_res.m_shader->uniforms)
            m_res.m_uniformsData.push_back(element.defaultValue);
    }

    void Material::Bind(Texture* p_emptyTexture)
    {
        if (HasShader())
        {
            using namespace LittleEngine;
            using namespace Resources;

            m_res.m_shader->Bind();

            int textureSlot = 0;

            for (auto& item : m_res.m_uniformsData)
            {
                auto name = item.m_key;
                auto uniformData = m_res.m_shader->GetUniformInfo(name);
                if (uniformData)
                {
                    switch (uniformData->type)
                    {
                    case UniformType::UNIFORM_BOOL: m_res.m_shader->SetUniformInt(
                            name, item.GetBool());
                        break;
                    case UniformType::UNIFORM_INT: m_res.m_shader->SetUniformInt(
                            name, item.GetInt());
                        break;
                    case UniformType::UNIFORM_FLOAT:  m_res.m_shader->SetUniformFloat(
                            name, item.GetFloat());
                        break;
                    case UniformType::UNIFORM_FLOAT_VEC2:  m_res.m_shader->
                            SetUniformVec2(name, item.GetVector2());
                        break;
                    case UniformType::UNIFORM_FLOAT_VEC3:  m_res.m_shader->
                            SetUniformVec3(name, item.GetVector3());
                        break;
                    case UniformType::UNIFORM_FLOAT_VEC4: m_res.m_shader->
                            SetUniformVec4(name, item.GetVector4());
                        break;
                    case UniformType::UNIFORM_SAMPLER_2D:
                        {
                            auto texPtr = item.m_texture;
                            if (!texPtr.IsNull())
                            {
                                texPtr->Bind(textureSlot);
                                m_res.m_shader->SetUniformInt(uniformData->name, textureSlot++);
                            }
                            else if (p_emptyTexture)
                            {
                                p_emptyTexture->Bind(textureSlot);
                                m_res.m_shader->SetUniformInt(uniformData->name, textureSlot++);
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
            m_res.m_shader->Unbind();
    }

    Shader*& Material::GetShader()
    {
        return m_res.m_shader.GetPtrReference();
    }

    bool Material::HasShader() const
    {
        return m_res.m_shader;
    }

    void Material::SetBlendable(bool p_transparent)
    {
        m_res.m_blendable = p_transparent;
    }

    void Material::SetBackfaceCulling(bool p_backfaceCulling)
    {
        m_res.m_backfaceCulling = p_backfaceCulling;
    }

    void Material::SetFrontfaceCulling(bool p_frontfaceCulling)
    {
        m_res.m_frontfaceCulling = p_frontfaceCulling;
    }

    void Material::SetDepthTest(bool p_depthTest)
    {
        m_res.m_depthTest = p_depthTest;
    }

    void Material::SetDepthWriting(bool p_depthWriting)
    {
        m_res.m_depthWriting = p_depthWriting;
    }

    void Material::SetColorWriting(bool p_colorWriting)
    {
        m_res.m_colorWriting = p_colorWriting;
    }

    void Material::SetGPUInstances(int p_instances)
    {
        m_res.m_gpuInstances = p_instances;
    }

    bool Material::IsBlendable() const
    {
        return m_res.m_blendable;
    }

    bool Material::HasBackfaceCulling() const
    {
        return m_res.m_backfaceCulling;
    }

    bool Material::HasFrontfaceCulling() const
    {
        return m_res.m_frontfaceCulling;
    }

    bool Material::HasDepthTest() const
    {
        return m_res.m_depthTest;
    }

    bool Material::HasDepthWriting() const
    {
        return m_res.m_depthWriting;
    }

    bool Material::HasColorWriting() const
    {
        return m_res.m_colorWriting;
    }

    int Material::GetGPUInstances() const
    {
        return m_res.m_gpuInstances;
    }

    uint8_t Material::GenerateStateMask() const
    {
        uint8_t result = 0;

        if (m_res.m_depthWriting) result |= 0b0000'0001;
        if (m_res.m_colorWriting) result |= 0b0000'0010;
        if (m_res.m_blendable) result |= 0b0000'0100;
        if (m_res.m_backfaceCulling || m_res.m_frontfaceCulling) result |= 0b0000'1000;
        if (m_res.m_depthTest) result |= 0b0001'0000;
        if (m_res.m_backfaceCulling) result |= 0b0010'0000;
        if (m_res.m_frontfaceCulling) result |= 0b0100'0000;

        return result;
    }

    std::vector<ResUniformInfo>& Material::GetUniformsData()
    {
        return m_res.m_uniformsData;
    }


}
