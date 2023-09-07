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
	* Represent a physical object with a box shape
	*/
	REFLECTION_TYPE(CPhysicalBox)
	CLASS (CPhysicalBox : public CPhysicalObject, WhiteListFields)
	{
		REFLECTION_BODY(CPhysicalBox)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;
		CPhysicalBox() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines the size of the box shape
		* @param p_size
		*/
		void SetSize(const LittleEngine::FVector3& p_size);

		/**
		* Returns the size of the box shape
		*/
		LittleEngine::FVector3 GetSize() const;

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
