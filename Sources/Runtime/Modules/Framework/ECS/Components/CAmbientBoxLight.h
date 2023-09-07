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
	* Represents an ambient light with a box shape
	*/
	REFLECTION_TYPE(CAmbientBoxLight)
	CLASS (CAmbientBoxLight : public CLight, WhiteListFields)
	{
		REFLECTION_BODY(CAmbientBoxLight)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CAmbientBoxLight() = default;
		void DoInit(ActorPtr p_owner) override;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Returns the size of the box
		*/
		LittleEngine::FVector3 GetSize() const;

		/**
		* Defines the size of the box
		* @param p_size
		*/
		void SetSize(const LittleEngine::FVector3& p_size);

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
