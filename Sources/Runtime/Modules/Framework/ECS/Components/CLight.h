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
	REFLECTION_COMPONENT_TYPE(CLight)
	CLASS (CLight : public Component, WhiteListFields,CustomEditor,Abstract)
	{
		REFLECTION_COMPONENT_BODY(CLight)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void OnAwake() override;
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


		virtual void OnAfterSceneLoaded(ActorPtr p_actor) override;


		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;
		
	protected:
		META(Enable)
		LittleEngine::Rendering::Entities::Light m_data;
	};
}
