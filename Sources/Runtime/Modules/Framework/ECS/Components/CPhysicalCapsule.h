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
	* Represent a physical object with a capsule hape
	*/
	REFLECTION_COMPONENT_TYPE(CPhysicalCapsule)
	CLASS (CPhysicalCapsule : public CPhysicalObject, WhiteListFields)
	{
		REFLECTION_COMPONENT_BODY(CPhysicalCapsule)
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
