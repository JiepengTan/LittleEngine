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
	REFLECTION_TYPE(CLight)
	CLASS (CLight : public Component, WhiteListFields)
	{
		REFLECTION_BODY(CLight)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;
		CLight();
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
		
	protected:
		LittleEngine::Rendering::Entities::Light m_data;
	};
}
