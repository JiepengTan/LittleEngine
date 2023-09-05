/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <sstream>
#include <fstream>

#include <GL/glew.h>

#include "Core/CoreInclude.h"

#include "Modules/Rendering/Resources/Loaders/ShaderLoader.h"

#include "Core/Tools/Utils/PathParser.h"

std::string LittleEngine::Rendering::Resources::Loaders::ShaderLoader::__FILE_TRACE;

LittleEngine::Rendering::Resources::Shader* LittleEngine::Rendering::Resources::Loaders::ShaderLoader::Create(const std::string& p_filePath)
{
	__FILE_TRACE = p_filePath;

	std::pair<std::string, std::string> source = ParseShader(p_filePath);

	uint32_t programID = CreateProgram(source.first, source.second);

	if (programID)
		return new Shader(p_filePath, programID);

	return nullptr;
}

LittleEngine::Rendering::Resources::Shader* LittleEngine::Rendering::Resources::Loaders::ShaderLoader::CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader)
{
	uint32_t programID = CreateProgram(p_vertexShader, p_fragmentShader);

	if (programID)
		return new Shader("", programID);

	return nullptr;
}

void LittleEngine::Rendering::Resources::Loaders::ShaderLoader::Recompile(Shader& p_shader, const std::string& p_filePath)
{
	__FILE_TRACE = p_filePath;

	std::pair<std::string, std::string> source = ParseShader(p_filePath);

	/* Create the new program */
	uint32_t newProgram = CreateProgram(source.first, source.second);

	if (newProgram)
	{
		/* Pointer to the shaderID (const data member, tricks to access it) */
		std::uint32_t* shaderID = reinterpret_cast<uint32_t*>(&p_shader) + offsetof(Shader, id);

		/* Deletes the previous program */
		glDeleteProgram(*shaderID);

		/* Store the new program in the shader */
		*shaderID = newProgram;

		p_shader.QueryUniforms();

		OVLOG_INFO("[COMPILE] \"" + __FILE_TRACE + "\": Success!");
	}
	else
	{
		OVLOG_ERROR("[COMPILE] \"" + __FILE_TRACE + "\": Failed! Previous shader version keept");
	}
}

bool LittleEngine::Rendering::Resources::Loaders::ShaderLoader::Destroy(Shader*& p_shader)
{
	if (p_shader)
	{
		delete p_shader;
		p_shader = nullptr;

		return true;
	}
	
	return false;
}

const std::string INCLUDE_PREFIX = "#include";
void ReadFileAndHandleIncludes(std::stringstream& output, std::string p_filepath)
{
	std::ifstream stream(p_filepath);

	std::string line;

	while (std::getline(stream, line))
	{
		size_t lineSize = line.size();
		size_t includePrefixSize = INCLUDE_PREFIX.size();
		// TODO deal duplicate include
		if (lineSize > includePrefixSize && line.find(INCLUDE_PREFIX) != std::string::npos)
		{
			std::string toIncludeFilePath = line.substr(includePrefixSize + 1, lineSize - includePrefixSize - 2);
			std::string fullPath = LittleEngine::Utils::PathParser::GetContainingFolder(p_filepath) + toIncludeFilePath;
			ReadFileAndHandleIncludes(output, fullPath);
		}
		else
		{
			output << line << std::endl;
		}
	}
}

std::pair<std::string, std::string> LittleEngine::Rendering::Resources::Loaders::ShaderLoader::ParseShader(const std::string& p_filePath)
{
	std::stringstream source;
	ReadFileAndHandleIncludes(source, p_filePath);
	
	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	std::string line;

	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (std::getline(source, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)			type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)	type = ShaderType::FRAGMENT;
		}
		else if (type != ShaderType::NONE)
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	return 
	{ 
		ss[static_cast<int>(ShaderType::VERTEX)].str(),
		ss[static_cast<int>(ShaderType::FRAGMENT)].str()
	};
}

uint32_t LittleEngine::Rendering::Resources::Loaders::ShaderLoader::CreateProgram(const std::string& p_vertexShader, const std::string& p_fragmentShader)
{
	const uint32_t program = glCreateProgram();

	const uint32_t vs = CompileShader(GL_VERTEX_SHADER, p_vertexShader);
	const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, p_fragmentShader);

	if (vs == 0 || fs == 0)
		return 0;

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

		OVLOG_ERROR("[LINK] \"" + __FILE_TRACE + "\":\n" + errorLog);

		glDeleteProgram(program);

		return 0;
	}

	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

uint32_t LittleEngine::Rendering::Resources::Loaders::ShaderLoader::CompileShader(uint32_t p_type, const std::string& p_source)
{
	const uint32_t id = glCreateShader(p_type);

	const char* src = p_source.c_str();

	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());

		std::string shaderTypeString = p_type == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER";
		std::string errorHeader = "[" + shaderTypeString + "] \"";
		OVLOG_ERROR(errorHeader + __FILE_TRACE + "\":\n" + errorLog);

		glDeleteShader(id);

		return 0;
	}

	return id;
}