/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Audio/Entities/AudioSource.h"
#include "Core/Tools/Eventing/Event.h"

#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* Represents an audio source. Its position in the world is important if the spatial sound settings is on
	*/
	REFLECTION_COMPONENT_TYPE(CAudioSource)
	CLASS (CAudioSource : public Component, WhiteListFields,CustomEditor)
	{
		REFLECTION_COMPONENT_BODY(CAudioSource)
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
		* Defines the sound to play on the audio source
		* @param p_sound
		*/
		void SetSound(LittleEngine::Sound* p_sound);

		/**
		* Defines if the audio source should autoplay (Play sound on enable)
		*/
		void SetAutoplay(bool p_autoplay);

		/**
		* Defines the audio source volume
		* @param p_volume
		*/
		void SetVolume(float p_volume);

		/**
		* Defines the audio source pan (Left / Right)
		* @param p_pan
		*/
		void SetPan(float p_pan);

		/**
		* Defines if the audio source should loop
		* @param p_looped
		*/
		void SetLooped(bool p_looped);

		/**
		* Defines the audio source pitch (= frequency or playback speed)
		* @param p_pitch
		*/
		void SetPitch(float p_pitch);

		/**
		* Defines if the audio source should be spatialized or not
		* @param p_value
		*/
		void SetSpatial(bool p_value);

		/**
		* Defines the audio source attenuation threshold (Minimum distance before applying sound attenuation)
		* @param p_distance
		*/
		void SetAttenuationThreshold(float p_distance);

		/**
		* Returns the sound attached to the audio source
		*/
		LittleEngine::Sound* GetSound() const;

		/**
		* Returns true if the audio source should call "Play" OnEnable
		*/
		bool IsAutoplayed() const;

		/**
		* Returns the audio source volume
		*/
		float GetVolume() const;

		/**
		* Returns the audio source pan (Left / Right)
		*/
		float GetPan() const;

		/**
		* Returns if the audio source is looping
		*/
		bool IsLooped() const;

		/**
		* Returns the audio source pitch (= frequency or playback speed)
		*/
		float GetPitch() const;

		/**
		* Returns true if the audio source sound has finished
		*/
		bool IsFinished() const;

		/**
		* Returns true if the audio source is spatialized
		*/
		bool IsSpatial() const;

		/**
		* Returns the audio source attenuation threshold (Minimum distance before applying sound attenuation)
		*/
		float GetAttenuationThreshold() const;

		/**
		* Play the audio source attached sound
		*/
		void Play();

		/**
		* Resume the audio source
		*/
		void Resume();

		/**
		* Pause the audio source
		*/
		void Pause();

		/**
		* Stop the audio source
		*/
		void Stop();

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

		virtual void OnEnable() override;
		virtual void OnDisable() override;
	private:
		LittleEngine::Audio::Entities::AudioSource* GetSource();
	private:
		LittleEngine::Sound* m_sound = nullptr;
		std::unique_ptr<LittleEngine::Audio::Entities::AudioSource> m_audioSource;
		bool m_autoPlay = false;
	};
}
