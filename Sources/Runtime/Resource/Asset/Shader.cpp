/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "GL/glew.h"

#include "Core/CoreInclude.h"

#include "Resource/Asset/Texture.h"
#include "Resource/Asset/Shader.h"

LittleEngine::Shader::Shader(const std::string p_path, uint32_t p_id) : path(p_path), id(p_id)
{
	QueryUniforms();
}

LittleEngine::Shader::~Shader()
{
	glDeleteProgram(id);
}

void LittleEngine::Shader::Bind() const
{
	glUseProgram(id);
}

void LittleEngine::Shader::Unbind() const
{
	glUseProgram(0);
}

void LittleEngine::Shader::SetUniformInt(const std::string& p_name, int p_value)
{
	glUniform1i(GetUniformLocation(p_name), p_value);
}

void LittleEngine::Shader::SetUniformFloat(const std::string& p_name, float p_value)
{
	glUniform1f(GetUniformLocation(p_name), p_value);
}

void LittleEngine::Shader::SetUniformVec2(const std::string & p_name, const LittleEngine::FVector2 & p_vec2)
{
	glUniform2f(GetUniformLocation(p_name), p_vec2.x, p_vec2.y);
}

void LittleEngine::Shader::SetUniformVec3(const std::string& p_name, const LittleEngine::FVector3& p_vec3)
{
	glUniform3f(GetUniformLocation(p_name), p_vec3.x, p_vec3.y, p_vec3.z);
}

void LittleEngine::Shader::SetUniformVec4(const std::string& p_name, const LittleEngine::FVector4& p_vec4)
{
	glUniform4f(GetUniformLocation(p_name), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w);
}

void LittleEngine::Shader::SetUniformMat4(const std::string& p_name, const LittleEngine::FMatrix4& p_mat4)
{
	glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_TRUE, &p_mat4.data[0]);
}
void LittleEngine::Shader::SetUniformMat4Array(const std::string& p_name, const std::vector<LittleEngine::FMatrix4>& p_matAry)
{
	for (auto i =0;i< p_matAry.size();i++)
	{
		auto mat = p_matAry[i];
		glUniformMatrix4fv(GetUniformLocation(p_name +"[" + std::to_string(i) + "]"), 1, GL_TRUE, &mat.data[0]);
	}
}

int LittleEngine::Shader::GetUniformInt(const std::string& p_name)
{
	int value;
	glGetUniformiv(id, GetUniformLocation(p_name), &value);
	return value;
}

float LittleEngine::Shader::GetUniformFloat(const std::string& p_name)
{
	float value;
	glGetUniformfv(id, GetUniformLocation(p_name), &value);
	return value;
}

LittleEngine::FVector2 LittleEngine::Shader::GetUniformVec2(const std::string& p_name)
{
	GLfloat values[2];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FVector2&>(values);
}

LittleEngine::FVector3 LittleEngine::Shader::GetUniformVec3(const std::string& p_name)
{
	GLfloat values[3];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FVector3&>(values);
}

LittleEngine::FVector4 LittleEngine::Shader::GetUniformVec4(const std::string& p_name)
{
	GLfloat values[4];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FVector4&>(values);
}

LittleEngine::FMatrix4 LittleEngine::Shader::GetUniformMat4(const std::string& p_name)
{
	GLfloat values[16];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<LittleEngine::FMatrix4&>(values);
}

bool LittleEngine::Shader::IsEngineUBOMember(const std::string & p_uniformName)
{
	return p_uniformName.rfind("ubo_", 0) == 0;
}

uint32_t LittleEngine::Shader::GetUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache.at(name);

	const int location = glGetUniformLocation(id, name.c_str());

	if (location == -1)
		LOG_WARNING("Uniform: '" + name + "' doesn't exist");

	m_uniformLocationCache[name] = location;

	return location;
}

void LittleEngine::Shader::QueryUniforms()
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
		Resources::UniformInfo info;
		if (!IsEngineUBOMember(name))
		{
			std::any defaultValue;
			bool hasValue = false;
			UniformType utype = static_cast<UniformType>(type);
			switch (utype)
			{
				case UniformType::UNIFORM_BOOL:			hasValue = true;info.defaultValue.SetBool(GetUniformInt(name))  ;	break;
				case UniformType::UNIFORM_INT:			hasValue = true;info.defaultValue.SetInt(GetUniformInt(name))  ;	break;
				case UniformType::UNIFORM_FLOAT:		hasValue = true;info.defaultValue.SetFloat(GetUniformFloat(name));	break;
				case UniformType::UNIFORM_FLOAT_VEC2:	hasValue = true;info.defaultValue.SetVector2(GetUniformVec2(name)) ;		break;
				case UniformType::UNIFORM_FLOAT_VEC3:	hasValue = true;info.defaultValue.SetVector3(GetUniformVec3(name)) ;		break;
				case UniformType::UNIFORM_FLOAT_VEC4:	hasValue = true;info.defaultValue.SetVector4(GetUniformVec4(name)) ;		break;
				case UniformType::UNIFORM_SAMPLER_2D:	hasValue = true;info.defaultValue.SetTexture(nullptr); 	break;
			}

			if (hasValue)
			{
				info.name = name;
				info.type = utype;
				info.location = GetUniformLocation(nameData.data());
				info.defaultValue.m_key = name;
				info.defaultValue.m_uniformType = utype;
				uniforms.push_back(info);
			}
		}
	}
}

const LittleEngine::Resources::UniformInfo* LittleEngine::Shader::GetUniformInfo(const std::string& p_name) const
{
	auto found = std::find_if(uniforms.begin(), uniforms.end(), [&p_name](const Resources::UniformInfo& p_element)
	{
		return p_name == p_element.name;
	});

	if (found != uniforms.end())
		return &*found;
	else
		return nullptr;
}
