/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Entities/Light.h"

#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Base class for any light
	*/
	class CLight : public Component
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CLight(Actor& p_owner);
		CLight() = default;
		/**
		* Returns light data
		*/
		const LittleEngine::Rendering::Entities::Light& GetData() const;

		/**
		* Returns light color
		*/
		const LittleEngine::FVector3& GetColor() const;

		/**
		* Returns light intensity
		*/
		float GetIntensity() const;

		/**
		* Returns light type
		*/
		LittleEngine::Rendering::Entities::Light::Type GetLightType();
		
		/**
		* Is directional light
		*/
		bool IsDirectional() ;
		
		/**
		* Defines a new color for the light
		* @param p_color
		*/
		void SetColor(const LittleEngine::FVector3& p_color);

		/**
		* Defines the intensity for the light
		* @param p_intensity
		*/
		void SetIntensity(float p_intensity);

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root) override;
		
	protected:
		LittleEngine::Rendering::Entities::Light m_data;
	};
}
