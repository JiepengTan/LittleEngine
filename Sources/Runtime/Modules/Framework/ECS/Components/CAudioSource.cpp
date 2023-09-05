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

LittleEngine::CAudioSource::CAudioSource(Actor& p_owner) :
	Component(p_owner),
	m_audioSource(LittleEngine::Global::ServiceLocator::Get<LittleEngine::Audio::Core::AudioPlayer>(), owner->transform.GetFTransform())
{
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
	m_audioSource.SetVolume(p_volume);
}

void LittleEngine::CAudioSource::SetPan(float p_pan)
{
	m_audioSource.SetPan(p_pan);
}

void LittleEngine::CAudioSource::SetLooped(bool p_looped)
{
	m_audioSource.SetLooped(p_looped);
}

void LittleEngine::CAudioSource::SetPitch(float p_pitch)
{
	m_audioSource.SetPitch(p_pitch);
}

void LittleEngine::CAudioSource::SetSpatial(bool p_value)
{
	m_audioSource.SetSpatial(p_value);
}

void LittleEngine::CAudioSource::SetAttenuationThreshold(float p_distance)
{
	m_audioSource.SetAttenuationThreshold(p_distance);
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
	return m_audioSource.GetVolume();
}

float LittleEngine::CAudioSource::GetPan() const
{
	return m_audioSource.GetPan();
}

bool LittleEngine::CAudioSource::IsLooped() const
{
	return m_audioSource.IsLooped();
}

float LittleEngine::CAudioSource::GetPitch() const
{
	return m_audioSource.GetPitch();
}

bool LittleEngine::CAudioSource::IsFinished() const
{
	return m_audioSource.IsFinished();
}

bool LittleEngine::CAudioSource::IsSpatial() const
{
	return m_audioSource.IsSpatial();
}

float LittleEngine::CAudioSource::GetAttenuationThreshold() const
{
	return m_audioSource.GetAttenuationThreshold();
}

void LittleEngine::CAudioSource::Play()
{
	if (owner->IsActive() && m_sound)
		m_audioSource.Play(*m_sound);
}

void LittleEngine::CAudioSource::Pause()
{
	if (owner->IsActive())
		m_audioSource.Pause();
}

void LittleEngine::CAudioSource::Resume()
{
	if (owner->IsActive())
		m_audioSource.Resume();
}

void LittleEngine::CAudioSource::Stop()
{
	if (owner->IsActive())
		m_audioSource.Stop();
}

void LittleEngine::CAudioSource::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	using namespace LittleEngine::Helpers;

	Serializer::SerializeBoolean(p_doc, p_node, "autoplay", m_autoPlay);
	Serializer::SerializeBoolean(p_doc, p_node, "spatial", IsSpatial());
	Serializer::SerializeFloat(p_doc, p_node, "volume", GetVolume());
	Serializer::SerializeFloat(p_doc, p_node, "pan", GetPan());
	Serializer::SerializeBoolean(p_doc, p_node, "looped", IsLooped());
	Serializer::SerializeFloat(p_doc, p_node, "pitch", GetPitch());
	Serializer::SerializeFloat(p_doc, p_node, "attenuation_threshold", GetAttenuationThreshold());
	Serializer::SerializeSound(p_doc, p_node, "audio_clip", m_sound);
}

void LittleEngine::CAudioSource::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	using namespace LittleEngine::Helpers;

	Serializer::DeserializeBoolean(p_doc, p_node, "autoplay", m_autoPlay);
	SetSpatial(Serializer::DeserializeBoolean(p_doc, p_node, "spatial"));
	SetVolume(Serializer::DeserializeFloat(p_doc, p_node, "volume"));
	SetPan(Serializer::DeserializeFloat(p_doc, p_node, "pan"));
	SetLooped(Serializer::DeserializeBoolean(p_doc, p_node, "looped"));
	SetPitch(Serializer::DeserializeFloat(p_doc, p_node, "pitch"));
	SetAttenuationThreshold(Serializer::DeserializeFloat(p_doc, p_node, "attenuation_threshold"));
	Serializer::DeserializeSound(p_doc, p_node, "audio_clip", m_sound);
}

void LittleEngine::CAudioSource::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
{
	using namespace LittleEngine::Audio::Entities;
	using namespace LittleEngine::Helpers;

	GUIDrawer::DrawSound(p_root, "Sound", m_sound);
	GUIDrawer::DrawBoolean(p_root, "Auto-play", m_autoPlay);
	GUIDrawer::DrawScalar<float>(p_root, "Volume", std::bind(&CAudioSource::GetVolume, this), std::bind(&CAudioSource::SetVolume, this, std::placeholders::_1), 0.01f, 0.0f, 1.0f);
	GUIDrawer::DrawScalar<float>(p_root, "Pan", std::bind(&CAudioSource::GetPan, this), std::bind(&CAudioSource::SetPan, this, std::placeholders::_1), 0.01f, -1.0f, 1.0f);
	GUIDrawer::DrawBoolean(p_root, "Looped", std::bind(&CAudioSource::IsLooped, this), std::bind(&CAudioSource::SetLooped, this, std::placeholders::_1));
	GUIDrawer::DrawScalar<float>(p_root, "Pitch", std::bind(&CAudioSource::GetPitch, this), std::bind(&CAudioSource::SetPitch, this, std::placeholders::_1), 0.01f, 0.01f, 10000.0f);
	GUIDrawer::DrawBoolean(p_root, "Spatial", std::bind(&CAudioSource::IsSpatial, this), std::bind(&CAudioSource::SetSpatial, this, std::placeholders::_1));
	GUIDrawer::DrawScalar<float>(p_root, "Attenuation threshold", std::bind(&CAudioSource::GetAttenuationThreshold, this), std::bind(&CAudioSource::SetAttenuationThreshold, this, std::placeholders::_1), 0.5f);

	GUIDrawer::CreateTitle(p_root, "Spatial graph");
	auto& graph = p_root.CreateWidget<LittleEngine::UI::Widgets::Plots::PlotLines>(std::vector<float>(), -0.1f, 1.1f);

	graph.size.y = 75.0f;

	auto& graphDispacther = graph.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::vector<float>>>();
	graphDispacther.RegisterGatherer([this, &graph]()->std::vector<float>
	{ 
		auto lerp = [](float a, float b, float t) { return a + t * (b - a); };

		std::vector<float> result;

		LittleEngine::FVector3 listenerPosition(0.0f, 0.0f, 0.0f);
		bool playMode = LittleEngine::Global::ServiceLocator::Get<SceneSystem::SceneManager>().GetCurrentScene()->IsPlaying();
		auto listenerInfo = LittleEngine::Global::ServiceLocator::Get<LittleEngine::Audio::Core::AudioEngine>().GetListenerInformation(!playMode);
		if (listenerInfo.has_value())
			listenerPosition = listenerInfo.value().first;

		float distanceToListener = LittleEngine::FVector3::Distance(listenerPosition, owner->transform.GetWorldPosition());

		for (float graphX = 0.0f; graphX < 50.0f; graphX += 0.25f)
		{
			float graphY = graphX < m_audioSource.GetAttenuationThreshold() ? 1.0f : 1.0f / (1.0f + 1.0f * (graphX - m_audioSource.GetAttenuationThreshold()));

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
	m_audioSource.Stop();
}
