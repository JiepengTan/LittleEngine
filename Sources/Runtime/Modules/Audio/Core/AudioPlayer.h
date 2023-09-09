/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "Modules/Audio/Core/AudioEngine.h"
#include "Resource/Asset/Sound.h"

namespace LittleEngine::Audio::Core
{
	/**
	* Handle sounds playback
	*/
	class AudioPlayer
	{
	public:
		/**
		* Constructor
		* @param p_audioEngine
		*/
		AudioPlayer(AudioEngine& p_audioEngine);

		/**
		* Play a sound in 2D and return a SoundTracker if tracking is asked
		* @param p_sound
		* @param p_autoPlay
		* @param p_looped
		* @param p_track
		*/
		std::unique_ptr<Tracking::SoundTracker> PlaySound(const Sound& p_sound, bool p_autoPlay = true, bool p_looped = false, bool p_track = false);

		/**
		* Play a sound in 3D and return a SoundTracker if tracking is asked
		* @param p_sound
		* @param p_autoPlay
		* @param p_looped
		* @param p_position
		* @param p_track
		*/
		std::unique_ptr<Tracking::SoundTracker> PlaySpatialSound(const Sound& p_sound, bool p_autoPlay = true, bool p_looped = false, const LittleEngine::FVector3& p_position = {0.0f, 0.0f, 0.0f}, bool p_track = false);

	private:
		AudioEngine& m_audioEngine;
	};
}
