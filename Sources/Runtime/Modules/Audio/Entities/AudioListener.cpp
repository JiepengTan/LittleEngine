/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Audio/Entities/AudioListener.h"

LittleEngine::Eventing::Event<LittleEngine::Audio::Entities::AudioListener&>	LittleEngine::Audio::Entities::AudioListener::CreatedEvent;
LittleEngine::Eventing::Event<LittleEngine::Audio::Entities::AudioListener&>	LittleEngine::Audio::Entities::AudioListener::DestroyedEvent;

LittleEngine::Audio::Entities::AudioListener::AudioListener() :
	m_transform(new LittleEngine::FTransform()),
	m_internalTransform(true)
{
	Setup();
}
void LittleEngine::Audio::Entities::AudioListener::DoInit(LittleEngine::FTransform& p_transform) 
{
	m_transform = &p_transform;
	m_internalTransform = false;
	Setup();
}
LittleEngine::Audio::Entities::AudioListener::AudioListener(LittleEngine::FTransform& p_transform) 
{
	DoInit(p_transform);
}

LittleEngine::Audio::Entities::AudioListener::~AudioListener()
{
	DestroyedEvent.Invoke(*this);
}

LittleEngine::FTransform& LittleEngine::Audio::Entities::AudioListener::GetTransform()
{
	return *m_transform;
}

void LittleEngine::Audio::Entities::AudioListener::SetEnabled(bool p_enable)
{
	m_enabled = p_enable;
}

bool LittleEngine::Audio::Entities::AudioListener::IsEnabled() const
{
	return m_enabled;
}

void LittleEngine::Audio::Entities::AudioListener::Setup()
{
	CreatedEvent.Invoke(*this);
}
