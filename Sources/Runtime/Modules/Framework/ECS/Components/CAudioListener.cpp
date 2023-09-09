/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Components/CAudioListener.h"
#include "Modules/Framework/ECS/Actor.h"

void LittleEngine::CAudioListener::OnAwake()
{
	Component::OnAwake();
	m_audioListener.DoInit(GetActor()->transform->GetFTransform());
	m_audioListener.SetEnabled(false);
}
std::string LittleEngine::CAudioListener::GetName()
{
	return "Audio Listener";
}

void LittleEngine::CAudioListener::OnSerialize(ISerializer p_serializer)
{
}

void LittleEngine::CAudioListener::OnDeserialize(ISerializer p_serializer)
{
}

void LittleEngine::CAudioListener::OnInspector()
{

}

void LittleEngine::CAudioListener::OnEnable()
{
	m_audioListener.SetEnabled(true);
}

void LittleEngine::CAudioListener::OnDisable()
{
	m_audioListener.SetEnabled(false);
}
