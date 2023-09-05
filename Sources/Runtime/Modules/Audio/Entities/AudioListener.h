/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "Core/Maths/FVector3.h"
#include "Core/Maths/FTransform.h"

#include "Modules/Audio/Entities/AudioSource.h"

namespace LittleEngine::Audio::Entities
{
	/**
	* Represents the ears of your application.
	* You can have multiple ones but only the last created will be considered by the AudioEngine
	*/
	class AudioListener
	{
	public:
		/**
		* AudioListener constructor (Internal transform management)
		*/
		AudioListener();

		/**
		* AudioListener constructor (External transform management)
		* @param p_transform
		*/
		AudioListener(LittleEngine::FTransform& p_transform);

		/**
		* AudioListener destructor
		*/
		~AudioListener();

		/**
		* Returns the AudioListener FTransform
		*/
		LittleEngine::FTransform& GetTransform();

		/**
		* Enable or disable the audio listener
		* @param p_enable
		*/
		void SetEnabled(bool p_enable);

		/**
		* Returns true if the audio listener is enabled
		*/
		bool IsEnabled() const;

	private:
		void Setup();

	private:
		/* Transform stuff */
		LittleEngine::FTransform* const		m_transform;
		const bool						m_internalTransform;
		bool							m_enabled = true;

	public:
		static LittleEngine::Eventing::Event<AudioListener&>	CreatedEvent;
		static LittleEngine::Eventing::Event<AudioListener&>	DestroyedEvent;
	};
}
