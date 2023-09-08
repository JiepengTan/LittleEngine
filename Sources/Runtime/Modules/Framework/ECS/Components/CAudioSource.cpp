/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Plots/PlotLines.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/Audio/Core/AudioPlayer.h"

#include "Modules/Framework/ECS/Components/CAudioSource.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"



void LittleEngine::CAudioSource::DoInit(ActorPtr p_owner)
{
	Component::DoInit(p_owner);
	m_audioSource = MakeUniquePtr<LittleEngine::Audio::Entities::AudioSource>
	(GetGlobalService<LittleEngine::Audio::Core::AudioPlayer>(),
		GetActor()->transform->GetFTransform());
}


std::string LittleEngine::CAudioSource::GetName()
{
	return "Audio Source";
}

void LittleEngine::CAudioSource::SetSound(LittleEngine::Audio::Resources::Sound* p_sound)
{
	m_sound = p_sound;
}

void LittleEngine::CAudioSource::SetAutoplay(bool p_autoplay)
{
	m_autoPlay = p_autoplay;
}

void LittleEngine::CAudioSource::SetVolume(float p_volume)
{
	m_audioSource->SetVolume(p_volume);
}

void LittleEngine::CAudioSource::SetPan(float p_pan)
{
	m_audioSource->SetPan(p_pan);
}

void LittleEngine::CAudioSource::SetLooped(bool p_looped)
{
	m_audioSource->SetLooped(p_looped);
}

void LittleEngine::CAudioSource::SetPitch(float p_pitch)
{
	m_audioSource->SetPitch(p_pitch);
}

void LittleEngine::CAudioSource::SetSpatial(bool p_value)
{
	m_audioSource->SetSpatial(p_value);
}

void LittleEngine::CAudioSource::SetAttenuationThreshold(float p_distance)
{
	m_audioSource->SetAttenuationThreshold(p_distance);
}

LittleEngine::Audio::Resources::Sound* LittleEngine::CAudioSource::GetSound() const
{
	return m_sound;
}

bool LittleEngine::CAudioSource::IsAutoplayed() const
{
	return m_autoPlay;
}

float LittleEngine::CAudioSource::GetVolume() const
{
	return m_audioSource->GetVolume();
}

float LittleEngine::CAudioSource::GetPan() const
{
	return m_audioSource->GetPan();
}

bool LittleEngine::CAudioSource::IsLooped() const
{
	return m_audioSource->IsLooped();
}

float LittleEngine::CAudioSource::GetPitch() const
{
	return m_audioSource->GetPitch();
}

bool LittleEngine::CAudioSource::IsFinished() const
{
	return m_audioSource->IsFinished();
}

bool LittleEngine::CAudioSource::IsSpatial() const
{
	return m_audioSource->IsSpatial();
}

float LittleEngine::CAudioSource::GetAttenuationThreshold() const
{
	return m_audioSource->GetAttenuationThreshold();
}

void LittleEngine::CAudioSource::Play()
{
	if (GetActor()->IsActive() && m_sound)
		m_audioSource->Play(*m_sound);
}

void LittleEngine::CAudioSource::Pause()
{
	if (GetActor()->IsActive())
		m_audioSource->Pause();
}

void LittleEngine::CAudioSource::Resume()
{
	if (GetActor()->IsActive())
		m_audioSource->Resume();
}

void LittleEngine::CAudioSource::Stop()
{
	if (GetActor()->IsActive())
		m_audioSource->Stop();
}

void LittleEngine::CAudioSource::OnSerialize(ISerializer p_serializer)
{
	

	SerializeUtil::SerializeBoolean("autoplay", m_autoPlay);
	SerializeUtil::SerializeBoolean("spatial", IsSpatial());
	SerializeUtil::SerializeFloat("volume", GetVolume());
	SerializeUtil::SerializeFloat("pan", GetPan());
	SerializeUtil::SerializeBoolean("looped", IsLooped());
	SerializeUtil::SerializeFloat("pitch", GetPitch());
	SerializeUtil::SerializeFloat("attenuation_threshold", GetAttenuationThreshold());
	SerializeUtil::SerializeSound("audio_clip", m_sound);
}

void LittleEngine::CAudioSource::OnDeserialize(ISerializer p_serializer)
{
	

	SerializeUtil::DeserializeBoolean("autoplay", m_autoPlay);
	SetSpatial(SerializeUtil::DeserializeBoolean("spatial"));
	SetVolume(SerializeUtil::DeserializeFloat("volume"));
	SetPan(SerializeUtil::DeserializeFloat("pan"));
	SetLooped(SerializeUtil::DeserializeBoolean("looped"));
	SetPitch(SerializeUtil::DeserializeFloat("pitch"));
	SetAttenuationThreshold(SerializeUtil::DeserializeFloat("attenuation_threshold"));
	SerializeUtil::DeserializeSound("audio_clip", m_sound);
}

void LittleEngine::CAudioSource::OnInspector()
{
	using namespace LittleEngine::Audio::Entities;
	

	GUIUtil::DrawSound( "Sound", m_sound);
	GUIUtil::DrawBoolean( "Auto-play", m_autoPlay);
	GUIUtil::DrawScalar<float>( "Volume", std::bind(&CAudioSource::GetVolume, this), std::bind(&CAudioSource::SetVolume, this, std::placeholders::_1), 0.01f, 0.0f, 1.0f);
	GUIUtil::DrawScalar<float>( "Pan", std::bind(&CAudioSource::GetPan, this), std::bind(&CAudioSource::SetPan, this, std::placeholders::_1), 0.01f, -1.0f, 1.0f);
	GUIUtil::DrawBoolean( "Looped", std::bind(&CAudioSource::IsLooped, this), std::bind(&CAudioSource::SetLooped, this, std::placeholders::_1));
	GUIUtil::DrawScalar<float>( "Pitch", std::bind(&CAudioSource::GetPitch, this), std::bind(&CAudioSource::SetPitch, this, std::placeholders::_1), 0.01f, 0.01f, 10000.0f);
	GUIUtil::DrawBoolean( "Spatial", std::bind(&CAudioSource::IsSpatial, this), std::bind(&CAudioSource::SetSpatial, this, std::placeholders::_1));
	GUIUtil::DrawScalar<float>( "Attenuation threshold", std::bind(&CAudioSource::GetAttenuationThreshold, this), std::bind(&CAudioSource::SetAttenuationThreshold, this, std::placeholders::_1), 0.5f);

	GUIUtil::CreateTitle( "Spatial graph");
	auto& graph = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Plots::PlotLines>(std::vector<float>(), -0.1f, 1.1f);

	graph.size.y = 75.0f;

	auto& graphDispacther = graph.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::vector<float>>>();
	graphDispacther.RegisterGatherer([this, &graph]()->std::vector<float>
	{ 
		auto lerp = [](float a, float b, float t) { return a + t * (b - a); };

		std::vector<float> result;

		LittleEngine::FVector3 listenerPosition(0.0f, 0.0f, 0.0f);
		bool playMode = GetGlobalService<SceneManager>().GetCurrentScene()->IsPlaying();
		auto listenerInfo = GetGlobalService<LittleEngine::Audio::Core::AudioEngine>().GetListenerInformation(!playMode);
		if (listenerInfo.has_value())
			listenerPosition = listenerInfo.value().first;

		float distanceToListener = LittleEngine::FVector3::Distance(listenerPosition, GetActor()->transform->GetWorldPosition());

		for (float graphX = 0.0f; graphX < 50.0f; graphX += 0.25f)
		{
			float graphY = graphX < m_audioSource->GetAttenuationThreshold() ? 1.0f : 1.0f / (1.0f + 1.0f * (graphX - m_audioSource->GetAttenuationThreshold()));

			if (abs(graphX - distanceToListener) <= 0.25f)
			{
				graph.forceHover = static_cast<int>(graphX * 4.0f);
				graph.overlay = std::to_string(static_cast<int>(graphY * 100.0f)) + "%";
			}

			result.push_back(graphY);
		}

		return result;
	});
}

void LittleEngine::CAudioSource::OnEnable()
{
	if (m_autoPlay)
		Play();
}

void LittleEngine::CAudioSource::OnDisable()
{
	m_audioSource->Stop();
}
