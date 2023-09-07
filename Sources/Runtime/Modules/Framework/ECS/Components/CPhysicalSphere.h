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
	REFLECTION_TYPE(CPhysicalSphere)
	CLASS (CPhysicalSphere : public CPhysicalObject, WhiteListFields)
	{
		REFLECTION_BODY(CPhysicalSphere)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;
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
