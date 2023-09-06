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
	* Represents an ambient light with a sphere shape
	*/
	REFLECTION_TYPE(CAmbientSphereLight)
	CLASS (CAmbientSphereLight : public CLight, WhiteListFields)
	{
		REFLECTION_BODY(CAmbientSphereLight)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;
		CAmbientSphereLight() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Returns the radius of the sphere
		*/
		float GetRadius() const;

		/**
		* Defines the radius of the sphere
		* @param p_radius
		*/
		void SetRadius(float p_radius);

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
	};
}
