/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "GL/glew.h"

#include "Core/CoreInclude.h"

#include "Resource/Asset/Texture.h"
#include "Resource/Asset/Shader.h"

LittleEngine::Rendering::Resources::Shader::Shader(const std::string p_path, uint32_t p_id) : path(p_path), id(p_id)
{
	QueryUniforms();
}

LittleEngine::Rendering::Resources::Shader::~Shader()
{
	glDeleteProgram(id);
}

void LittleEngine::Rendering::Resources::Shader::Bind() const
{
	glUseProgram(id);
}

void LittleEngine::Rendering::Resources::Shader::Unbind() const
{
	glUseProgram(0);
}

void LittleEngine::Rendering::Resources::Shader::SetUniformInt(const std::string& p_name, int p_value)
{
	glUniform1i(GetUniformLocation(p_name), p_value);
}

void LittleEngine::Rendering::Resources::Shader::SetUniformFloat(const std::string& p_name, float p_value)
{
	glUniform1f(GetUniformLocation(p_name), p_value);
}

void LittleEngine::Rendering::Resources::Shader::SetUniformVec2(const std::string & p_name, const LittleEngine::FVector2 & p_vec2)
{
	glUniform2f(GetUniformLocation(p_name), p_vec2.x, p_vec2.y);
}

void LittleEngine::Rendering::Resources::Shader::SetUniformVec3(const std::string& p_name, const LittleEngine::FVector3& p_vec3)
{
	glUniform3f(GetUniformLocation(p_name), p_vec3.x, p_vec3.y, p_vec3.z);
}

void LittleEngine::Rendering::Resources::Shader::SetUniformVec4(const std::string& p_name, const LittleEngine::FVector4& p_vec4)
{
	glUniform4f(GetUniformLocation(p_name), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w);
}

void LittleEngine::Rendering::Resources::Shader::SetUniformMat4(const std::string& p_name, const LittleEngine::FMatrix4& p_mat4)
{
	glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_TRUE, &p_mat4.data[0]);
}
void LittleEngine::Rendering::Resources::Shader::SetUniformMat4Array(const std::string& p_name, const std::vector<LittleEngine::FMatrix4>& p_matAry)
{
	for (auto i =0;i< p_matAry.size();i++)
	{
		auto mat = p_matAry[i];
		glUniformMatrix4fv(GetUniformLocation(p_name +"[" + std::to_string(i) + "]"), 1, GL_TRUE, &mat.data[0]);
	}
}

int LittleEngine::Rendering::Resources::Shader::GetUniformInt(const std::string& p_name)
{
	int value;
	glGetUniformiv(id, GetUniformLocation(p_name), &value);
	return value;
}

float LittleEngine::Rendering::Resources::Shader::GetUniformFloat(const std::string& p_name)
{
	float value;
	glGetUniformfv(id, GetUniformLocation(p_name), &value);
	return value;
}

LittleEngine::FVector2 LittleEngine::Rendering::Resources::Shader::GetUniformVec2(const std::string& p_name)
{
	GLfloat values[2];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FVector2&>(values);
}

LittleEngine::FVector3 LittleEngine::Rendering::Resources::Shader::GetUniformVec3(const std::string& p_name)
{
	GLfloat values[3];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FVector3&>(values);
}

LittleEngine::FVector4 LittleEngine::Rendering::Resources::Shader::GetUniformVec4(const std::string& p_name)
{
	GLfloat values[4];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FVector4&>(values);
}

LittleEngine::FMatrix4 LittleEngine::Rendering::Resources::Shader::GetUniformMat4(const std::string& p_name)
{
	GLfloat values[16];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FMatrix4&>(values);
}

bool LittleEngine::Rendering::Resources::Shader::IsEngineUBOMember(const std::string & p_uniformName)
{
	return p_uniformName.rfind("ubo_", 0) == 0;
}

uint32_t LittleEngine::Rendering::Resources::Shader::GetUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache.at(name);

	const int location = glGetUniformLocation(id, name.c_str());

	if (location == -1)
		LOG_WARNING("Uniform: '" + name + "' doesn't exist");

	m_uniformLocationCache[name] = location;

	return location;
}

void LittleEngine::Rendering::Resources::Shader::QueryUniforms()
{
	GLint numActiveUniforms = 0;
	uniforms.clear();
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	std::vector<GLchar> nameData(256);
	for (int unif = 0; unif < numActiveUniforms; ++unif)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(id, unif, static_cast<GLsizei>(nameData.size()), &actualLength, &arraySize, &type, &nameData[0]);
		std::string name(static_cast<char*>(nameData.data()), actualLength);

		if (!IsEngineUBOMember(name))
		{
			std::any defaultValue;

			switch (static_cast<UniformType>(type))
			{
			case UniformType::UNIFORM_BOOL:			defaultValue = std::make_any<bool>(GetUniformInt(name));					break;
			case UniformType::UNIFORM_INT:			defaultValue = std::make_any<int>(GetUniformInt(name));						break;
			case UniformType::UNIFORM_FLOAT:		defaultValue = std::make_any<float>(GetUniformFloat(name));					break;
			case UniformType::UNIFORM_FLOAT_VEC2:	defaultValue = std::make_any<LittleEngine::FVector2>(GetUniformVec2(name));		break;
			case UniformType::UNIFORM_FLOAT_VEC3:	defaultValue = std::make_any<LittleEngine::FVector3>(GetUniformVec3(name));		break;
			case UniformType::UNIFORM_FLOAT_VEC4:	defaultValue = std::make_any<LittleEngine::FVector4>(GetUniformVec4(name));		break;
			case UniformType::UNIFORM_SAMPLER_2D:	defaultValue = std::make_any<LittleEngine::Rendering::Resources::Texture*>(nullptr);	break;
			}

			if (defaultValue.has_value())
			{
				uniforms.push_back
				({
					static_cast<UniformType>(type),
					name,
					GetUniformLocation(nameData.data()),
					defaultValue
				});
			}
		}
	}
}

const LittleEngine::Rendering::Resources::UniformInfo* LittleEngine::Rendering::Resources::Shader::GetUniformInfo(const std::string& p_name) const
{
	auto found = std::find_if(uniforms.begin(), uniforms.end(), [&p_name](const UniformInfo& p_element)
	{
		return p_name == p_element.name;
	});

	if (found != uniforms.end())
		return &*found;
	else
		return nullptr;
}
