/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "any"
#include "map"
#include "Resource/Asset/Texture.h"
#include "Resource/Asset/Shader.h"
#include "Modules/Framework/API/ISerializable.h"
#include "Resource/Data/ResIncludeRendering.h"

namespace LittleEngine
{
	/**
	* A material is a combination of a shader and some settings (Material settings and shader settings)
	*/
	REFLECTION_TYPE(Material)
	CLASS(Material, WhiteListFields)
	{
		REFLECTION_BODY(Material)
		
	public:
		Material() = default;
		virtual ~Material()= default;
		
		/**
		* Defines the shader to attach to this material instance
		* @param p_shader
		*/
		void SetShader(StringText p_path);
		void SetShader(Shader* p_shader);
		/**
		* Fill uniform with default uniform values
		*/
		void FillUniform();

		/**
		* Bind the material and send its uniform data to the GPU
		* @parma p_emptyTexture (The texture to use if a texture uniform is nullptr)
		*/
		void Bind(Texture* p_emptyTexture);

		/**
		* Unbind the material
		*/
		void UnBind();

		/**
		* Set a shader uniform value
		* @param p_key
		* @param p_value
		*/
		template< typename  T>
		void SetProperty(const std::string p_key, const T& p_value, bool p_isForce = false);

		/**
		* Set a shader uniform value
		* @param p_key
		*/
		template< typename  T>
		void GetProperty(const std::string p_key,T & val);

		ResUniformInfo* GetProperty(const std::string p_key);

		bool HasProperty(const std::string p_key) const;

		/**
		* Returns the attached shader
		*/
		Shader*& GetShader();

		/**
		* Returns true if the material has a shader attached
		*/
		bool HasShader() const;

		/**
		* Defines if the material is blendable
		* @param p_blendable
		*/
		void SetBlendable(bool p_blendable);

		/**
		* Defines if the material has backface culling
		* @param p_backfaceCulling
		*/
		void SetBackfaceCulling(bool p_backfaceCulling);

		/**
		* Defines if the material has frontface culling
		* @param p_frontfaceCulling
		*/
		void SetFrontfaceCulling(bool p_frontfaceCulling);

		/**
		* Defines if the material has depth test
		* @param p_depthTest
		*/
		void SetDepthTest(bool p_depthTest);

		/**
		* Defines if the material has depth writting
		* @param p_depthWriting
		*/
		void SetDepthWriting(bool p_depthWriting);

		/**
		* Defines if the material has color writting
		* @param p_colorWriting
		*/
		void SetColorWriting(bool p_colorWriting);

		/**
		* Defines the number of instances
		* @param p_instances
		*/
		void SetGPUInstances(int p_instances);

		/**
		* Returns true if the material is blendable
		*/
		bool IsBlendable() const;

		/**
		* Returns true if the material has backface culling
		*/
		bool HasBackfaceCulling() const;

		/**
		* Returns true if the material has frontface culling
		*/
		bool HasFrontfaceCulling() const;

		/**
		* Returns true if the material has depth test
		*/
		bool HasDepthTest() const;

		/**
		* Returns true if the material has depth writing
		*/
		bool HasDepthWriting() const;

		/**
		* Returns true if the material has color writing
		*/
		bool HasColorWriting() const;

		/**
		* Returns the number of instances
		*/
		int GetGPUInstances() const;

		/**
		* Generate an OpenGL state mask with the current material settings
		*/
		uint8_t GenerateStateMask() const;

		/**
		* Returns the uniforms data of the material
		*/
		std::vector<ResUniformInfo>& GetUniformsData();


	public:
		std::string path;

	private:
		META(Enable)
		ResMaterial m_res;
	};


}

#include "Resource/Asset/Material.inl"
