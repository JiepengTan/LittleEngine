/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Components/CAudioListener.h"
#include "Modules/Framework/ECS/Actor.h"

void LittleEngine::CAudioListener::DoInit(ActorPtr p_owner)
{
	Component::DoInit(p_owner);
	m_audioListener.DoInit(GetActor()->transform->GetFTransform());
	m_audioListener.SetEnabled(false);
}
std::string LittleEngine::CAudioListener::GetName()
{
	return "Audio Listener";
}

void LittleEngine::CAudioListener::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void LittleEngine::CAudioListener::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void LittleEngine::CAudioListener::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
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
