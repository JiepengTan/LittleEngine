/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/ECS/Components/AComponent.h"

#include "Modules/Framework/ECS/Components/CPhysicalObject.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represent a physical object with a capsule hape
	*/
	class CPhysicalCapsule : public CPhysicalObject
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CPhysicalCapsule(Actor& p_owner);
		CPhysicalCapsule() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines the radius of the capsule shape
		* @param p_radius
		*/
		void SetRadius(float p_radius);

		/**
		* Defines the height of the capsule shape
		* @param p_height
		*/
		void SetHeight(float p_height);

		/**
		* Returns the radius of the capsule shape
		*/
		float GetRadius() const;

		/**
		* Returns the height of the capsule shape
		*/
		float GetHeight() const;

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
