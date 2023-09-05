/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Audio/Tracking/SoundTracker.h"

LittleEngine::Audio::Tracking::SoundTracker::SoundTracker(irrklang::ISound* p_track) :
	m_track(p_track),
	m_soundStopEventBinder(StopEvent)
{
	// m_track->setSoundStopEventReceiver(&m_soundStopEventBinder);
}

LittleEngine::Audio::Tracking::SoundTracker::~SoundTracker()
{
	// m_track->setSoundStopEventReceiver(nullptr);
}

irrklang::ISound* LittleEngine::Audio::Tracking::SoundTracker::GetTrack() const
{
	return m_track;
}
