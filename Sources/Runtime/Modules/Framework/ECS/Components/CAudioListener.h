/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "AComponent.h"

#include "Core/Tools/Eventing/Event.h"

#include "Modules/Audio/Entities/AudioListener.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represents an audio listener. The distance between audio sources and audio listener will affect the sound
	* attenuation of spatialized sounds
	*/
	class CAudioListener : public AComponent
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CAudioListener(Actor& p_owner);
		CAudioListener() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

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

	private:
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		LittleEngine::Audio::Entities::AudioListener m_audioListener;
	};
}
