/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "unordered_map"

#include "Core/Maths/FVector2.h"
#include "Core/Maths/FVector3.h"
#include "Core/Maths/FVector4.h"
#include "Core/Maths/FMatrix4.h"

#include "Resource/Asset/UniformInfo.h"



namespace LittleEngine
{
	namespace Resources { class ShaderLoader; }

	/**
	* OpenGL shader program wrapper
	*/
	class Shader
	{
	friend class Resources::ShaderLoader;

	public:
		/**
		* Bind the program
		*/
		void Bind() const;

		/**
		* Unbind the program
		*/
		void Unbind() const;

		/**
		* Send a int to the GPU via a shader uniform
		* @param p_name
		* @param p_value
		*/
		void SetUniformInt(const std::string& p_name, int p_value);

		/**
		* Send a float to the GPU via a shader uniform
		* @param p_name
		* @param p_value
		*/
		void SetUniformFloat(const std::string& p_name, float p_value);

		/**
		* Send a vec2 to the GPU via a shader uniform
		* @param p_name
		* @param p_vec2
		*/
		void SetUniformVec2(const std::string& p_name, const LittleEngine::FVector2& p_vec2);

		/**
		* Send a vec3 to the GPU via a shader uniform
		* @param p_name
		* @param p_vec3
		*/
		void SetUniformVec3(const std::string& p_name, const LittleEngine::FVector3& p_vec3);

		/**
		* Send a vec4 to the GPU via a shader uniform
		* @param p_name
		* @param p_vec4
		*/
		void SetUniformVec4(const std::string& p_name, const LittleEngine::FVector4& p_vec4);

		/**
		* Send a mat4 to the GPU via a shader uniform
		* @param p_name
		* @param p_mat4
		*/
		void SetUniformMat4(const std::string& p_name, const LittleEngine::FMatrix4& p_mat4);
		void SetUniformMat4Array(const std::string& p_name, const std::vector<LittleEngine::FMatrix4>& p_matAry);
		/**
		* Returns the int uniform value identified by the given name
		* @param p_name
		*/
		int GetUniformInt(const std::string& p_name);

		/**
		* Returns the float uniform value identified by the given name
		* @param p_name
		*/
		float GetUniformFloat(const std::string& p_name);

		/**
		* Returns the vec2 uniform value identified by the given name
		* @param p_name
		*/
		LittleEngine::FVector2 GetUniformVec2(const std::string& p_name);

		/**
		* Returns the vec3 uniform value identified by the given name
		* @param p_name
		*/
		LittleEngine::FVector3 GetUniformVec3(const std::string& p_name);

		/**
		* Returns the vec4 uniform value identified by the given name
		* @param p_name
		*/
		LittleEngine::FVector4 GetUniformVec4(const std::string& p_name);

		/**
		* Returns the mat4 uniform value identified by the given name
		* @param p_name
		*/
		LittleEngine::FMatrix4 GetUniformMat4(const std::string& p_name);

		/**
		* Returns information about the uniform identified by the given name or nullptr if not found
		* @param p_name
		*/
		const Resources::UniformInfo* GetUniformInfo(const std::string& p_name) const;

		/**
		* Query the uniforms from the program and store them in the uniform vector
		*/
		void QueryUniforms();

	private:
		Shader(const std::string p_path, uint32_t p_id);
		~Shader();

		static bool IsEngineUBOMember(const std::string& p_uniformName);
		uint32_t GetUniformLocation(const std::string& name);

	public:
		const uint32_t id;
		std::string path;
		std::vector<Resources::UniformInfo> uniforms;

	private:
		std::unordered_map<std::string, int> m_uniformLocationCache;
	};
}
