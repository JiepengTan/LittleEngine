/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "sstream"
#include "fstream"

#include "GL/glew.h"

#include "Core/CoreInclude.h"

#include "Resource/Loader/ShaderLoader.h"

#include "Core/Tools/Utils/PathParser.h"
#include "Resource/Core/ResPtr.h"

std::string LittleEngine::Resources::ShaderLoader::__FILE_TRACE;

LittleEngine::Shader* LittleEngine::Resources::ShaderLoader::Create(const std::string& p_filePath)
{
	__FILE_TRACE = p_filePath;

	std::pair<std::string, std::string> source = ParseShader(p_filePath);

	uint32_t programID = CreateProgram(source.first, source.second);

	if (programID)
	{
		//LOG_WARNING("ShaderManager  programID= "+ std::to_string(programID) + " path"+ p_filePath);
		return new Shader(p_filePath, programID);
	}else
	{
		LOG_ERROR("Create Shader Failed " + p_filePath);
	}

	return nullptr;
}

LittleEngine::Shader* LittleEngine::Resources::ShaderLoader::CreateFromSource(const std::string& p_vertexShader, const std::string& p_fragmentShader,std::string name)
{
	uint32_t programID = CreateProgram(p_vertexShader, p_fragmentShader);

	if (programID){
		Shader* shader = new Shader(name, programID);
		ResourcesUtils::RegisterShader(name,shader);
	}
	return nullptr;
}

void LittleEngine::Resources::ShaderLoader::Recompile(Shader& p_shader, const std::string& p_filePath)
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

		LOG_INFO("[COMPILE] \"" + __FILE_TRACE + "\": Success!");
	}
	else
	{
		LOG_ERROR("[COMPILE] \"" + __FILE_TRACE + "\": Failed! Previous shader version keept");
	}
}

bool LittleEngine::Resources::ShaderLoader::Destroy(Shader*& p_shader)
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

std::pair<std::string, std::string> LittleEngine::Resources::ShaderLoader::ParseShader(const std::string& p_filePath)
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

uint32_t LittleEngine::Resources::ShaderLoader::CreateProgram(const std::string& p_vertexShader, const std::string& p_fragmentShader)
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

		LOG_ERROR("[LINK] \"" + __FILE_TRACE + "\":\n" + errorLog);

		glDeleteProgram(program);

		return 0;
	}

	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

uint32_t LittleEngine::Resources::ShaderLoader::CompileShader(uint32_t p_type, const std::string& p_source)
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
		LOG_ERROR(errorHeader + __FILE_TRACE + "\":\n" + errorLog);

		glDeleteShader(id);

		return 0;
	}

	return id;
}
