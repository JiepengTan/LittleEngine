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
	* A simple light that has no attenuation and that has a direction
	*/
	REFLECTION_TYPE(CDirectionalLight)
	CLASS (CDirectionalLight : public CLight, WhiteListFields)
	{
		REFLECTION_BODY(CDirectionalLight)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;
		CDirectionalLight() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

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
