/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Audio/Tracking/SoundStopEventBinder.h"

LittleEngine::Audio::Tracking::SoundStopEventBinder::SoundStopEventBinder(LittleEngine::Eventing::Event<>& p_soundFinishedEvent) : SoundFinishedEvent(p_soundFinishedEvent)
{
}

void LittleEngine::Audio::Tracking::SoundStopEventBinder::OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData)
{
	SoundFinishedEvent.Invoke();
}
