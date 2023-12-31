/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Audio/Core/AudioEngine.h"

#include "algorithm"

#include "irrklang/irrKlang.h"

LittleEngine::Audio::Core::AudioEngine::AudioEngine(const std::string & p_workingDirectory) : m_workingDirectory(p_workingDirectory)
{
	m_irrklangEngine = irrklang::createIrrKlangDevice();

	using AudioSourceReceiver	= void(AudioEngine::*)(LittleEngine::Audio::Entities::AudioSource&);
	using AudioListenerReceiver = void(AudioEngine::*)(LittleEngine::Audio::Entities::AudioListener&);

	Entities::AudioSource::CreatedEvent		+= std::bind(static_cast<AudioSourceReceiver>(&AudioEngine::Consider), this, std::placeholders::_1);
	Entities::AudioSource::DestroyedEvent	+= std::bind(static_cast<AudioSourceReceiver>(&AudioEngine::Unconsider), this, std::placeholders::_1);
	Entities::AudioListener::CreatedEvent	+= std::bind(static_cast<AudioListenerReceiver>(&AudioEngine::Consider), this, std::placeholders::_1);
	Entities::AudioListener::DestroyedEvent	+= std::bind(static_cast<AudioListenerReceiver>(&AudioEngine::Unconsider), this, std::placeholders::_1);
}

LittleEngine::Audio::Core::AudioEngine::~AudioEngine()
{
	m_irrklangEngine->drop();
}

void LittleEngine::Audio::Core::AudioEngine::Update()
{
	/* Update tracked sounds */
	std::for_each(m_audioSources.begin(), m_audioSources.end(), std::mem_fn(&Entities::AudioSource::UpdateTrackedSoundPosition));

	/* Defines the listener position using the last listener created (If any) */
	std::optional<std::pair<LittleEngine::FVector3, LittleEngine::FVector3>> listener = GetListenerInformation();
	if (listener.has_value())
	{
		m_irrklangEngine->setListenerPosition
		(
			reinterpret_cast<const irrklang::vec3df&>(listener.value().first),
			reinterpret_cast<const irrklang::vec3df&>(listener.value().second)
		);
	}
	else
	{
		m_irrklangEngine->setListenerPosition({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	}
}

void LittleEngine::Audio::Core::AudioEngine::Suspend()
{
	std::for_each(m_audioSources.begin(), m_audioSources.end(), [this](std::reference_wrapper<Entities::AudioSource> p_audioSource)
	{
		if (p_audioSource.get().IsTrackingSound() && !p_audioSource.get().GetTrackedSound()->GetTrack()->getIsPaused())
		{
			m_suspendedAudioSources.push_back(p_audioSource);
			p_audioSource.get().Pause();
		}
	});

	m_suspended = true;
}

void LittleEngine::Audio::Core::AudioEngine::Unsuspend()
{
	std::for_each(m_suspendedAudioSources.begin(), m_suspendedAudioSources.end(), std::mem_fn(&Entities::AudioSource::Resume));
	m_suspendedAudioSources.clear();
	m_suspended = false;
}

bool LittleEngine::Audio::Core::AudioEngine::IsSuspended() const
{
	return m_suspended;
}

const std::string& LittleEngine::Audio::Core::AudioEngine::GetWorkingDirectory() const
{
	return m_workingDirectory;
}

irrklang::ISoundEngine * LittleEngine::Audio::Core::AudioEngine::GetIrrklangEngine() const
{
	return m_irrklangEngine;
}

std::optional<std::pair<LittleEngine::FVector3, LittleEngine::FVector3>> LittleEngine::Audio::Core::AudioEngine::GetListenerInformation(bool p_considerDisabled) const
{
	for (auto listener : m_audioListeners)
	{
		if (listener.get().IsEnabled() || p_considerDisabled)
		{
			auto& transform = m_audioListeners.back().get().GetTransform();
			return
			{{
				transform.GetWorldPosition(),
				transform.GetWorldForward() * -1.0f
			}};
		}
	}

	return {};
}

void LittleEngine::Audio::Core::AudioEngine::Consider(LittleEngine::Audio::Entities::AudioSource & p_audioSource)
{
	m_audioSources.push_back(std::ref(p_audioSource));
}

void LittleEngine::Audio::Core::AudioEngine::Consider(LittleEngine::Audio::Entities::AudioListener & p_audioListener)
{
	m_audioListeners.push_back(std::ref(p_audioListener));
}

void LittleEngine::Audio::Core::AudioEngine::Unconsider(LittleEngine::Audio::Entities::AudioSource & p_audioSource)
{
	auto found = std::find_if(m_audioSources.begin(), m_audioSources.end(), [&p_audioSource](std::reference_wrapper<Entities::AudioSource> element)
	{
		return std::addressof(p_audioSource) == std::addressof(element.get());
	});

	if (found != m_audioSources.end())
		m_audioSources.erase(found);

	if (m_suspended)
	{
		auto found = std::find_if(m_suspendedAudioSources.begin(), m_suspendedAudioSources.end(), [&p_audioSource](std::reference_wrapper<Entities::AudioSource> element)
		{
			return std::addressof(p_audioSource) == std::addressof(element.get());
		});

		if (found != m_suspendedAudioSources.end())
			m_suspendedAudioSources.erase(found);
	}
}

void LittleEngine::Audio::Core::AudioEngine::Unconsider(LittleEngine::Audio::Entities::AudioListener & p_audioListener)
{
	auto found = std::find_if(m_audioListeners.begin(), m_audioListeners.end(), [&p_audioListener](std::reference_wrapper<Entities::AudioListener> element)
	{
		return std::addressof(p_audioListener) == std::addressof(element.get());
	});

	if (found != m_audioListeners.end())
		m_audioListeners.erase(found);
}
