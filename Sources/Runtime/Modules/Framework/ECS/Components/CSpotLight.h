/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/ECS/Components/CLight.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* A light that is localized, has attenuation and has a direction
	*/
	REFLECTION_COMPONENT_TYPE(CSpotLight)
	CLASS (CSpotLight : public CLight, WhiteListFields,CustomEditor)
	{
		REFLECTION_COMPONENT_BODY(CSpotLight)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void OnAwake() override;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Returns the light constant
		*/
		float GetConstant() const;

		/**
		* Returns the light linear
		*/
		float GetLinear() const;

		/**
		* Returns the light quadratic
		*/
		float GetQuadratic() const;

		/**
		* Returns the light cutoff
		*/
		float GetCutoff() const;

		/**
		* Returns the light outercutoff
		*/
		float GetOuterCutoff() const;

		/**
		* Defines the light constant
		* @param p_constant
		*/
		void SetConstant(float p_constant);

		/**
		* Defines the light linear
		* @param p_linear
		*/
		void SetLinear(float p_linear);

		/**
		* Defines the light quadratic
		* @param p_quadratic
		*/
		void SetQuadratic(float p_quadratic);

		/**
		* Defines the light cutoff
		* @param p_cutoff
		*/
		void SetCutoff(float p_cutoff);

		/**
		* Defines the light outercutoff
		* @param p_cutoff
		*/
		void SetOuterCutoff(float p_outerCutoff);

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(ISerializer p_serializer) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(ISerializer p_serializer) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;
	};
}
