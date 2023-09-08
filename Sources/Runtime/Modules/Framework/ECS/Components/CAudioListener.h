/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/ECS/Component.h"

#include "Core/Tools/Eventing/Event.h"

#include "Modules/Audio/Entities/AudioListener.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represents an audio listener. The distance between audio sources and audio listener will affect the sound
	* attenuation of spatialized sounds
	*/
	REFLECTION_COMPONENT_TYPE(CAudioListener)
	CLASS (CAudioListener : public Component, WhiteListFields)
	{
		REFLECTION_COMPONENT_BODY(CAudioListener)
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;

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

	public:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		LittleEngine::Audio::Entities::AudioListener m_audioListener;
	};
}
