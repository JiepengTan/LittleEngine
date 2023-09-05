/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/CoreInclude.h"

#include "Modules/Audio/Core/AudioPlayer.h"

LittleEngine::Audio::Core::AudioPlayer::AudioPlayer(AudioEngine& p_audioEngine) : m_audioEngine(p_audioEngine)
{
}

std::unique_ptr<LittleEngine::Audio::Tracking::SoundTracker> LittleEngine::Audio::Core::AudioPlayer::PlaySound(const Resources::Sound& p_sound, bool p_autoPlay, bool p_looped, bool p_track)
{
	std::unique_ptr<Tracking::SoundTracker> result;

	irrklang::ISound* sound = m_audioEngine.GetIrrklangEngine()->play2D((m_audioEngine.GetWorkingDirectory() + p_sound.path).c_str(), p_looped, p_autoPlay, p_track);

	if (p_track)
	{
		if (sound)
			result = std::make_unique<Tracking::SoundTracker>(sound);
		else
			OVLOG_ERROR("Unable to play \"" + p_sound.path + "\"");
	}

	return result;
}

std::unique_ptr<LittleEngine::Audio::Tracking::SoundTracker> LittleEngine::Audio::Core::AudioPlayer::PlaySpatialSound(const Resources::Sound& p_sound, bool p_autoPlay, bool p_looped, const LittleEngine::FVector3& p_position, bool p_track)
{
	std::unique_ptr<Tracking::SoundTracker> result;

	irrklang::ISound* sound = m_audioEngine.GetIrrklangEngine()->play3D((m_audioEngine.GetWorkingDirectory() + p_sound.path).c_str(), reinterpret_cast<const irrklang::vec3df&>(p_position), p_looped, p_autoPlay, p_track);

	if (p_track)
	{
		if (sound)
			result = std::make_unique<Tracking::SoundTracker>(sound);
		else
			OVLOG_ERROR("Unable to play \"" + p_sound.path + "\"");
	}

	return result;
}
