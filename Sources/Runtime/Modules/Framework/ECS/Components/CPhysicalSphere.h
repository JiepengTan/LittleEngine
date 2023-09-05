/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/ECS/Component.h"

#include "Modules/Framework/ECS/Components/CPhysicalObject.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represent a physical object with a sphere shape
	*/
	class CPhysicalSphere : public CPhysicalObject
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CPhysicalSphere(Actor& p_owner);
		CPhysicalSphere() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines the radius of the sphere shape
		* @param p_radius
		*/
		void SetRadius(float p_radius);

		/**
		* Returns the radius of the sphere shape
		*/
		float GetRadius() const;

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
