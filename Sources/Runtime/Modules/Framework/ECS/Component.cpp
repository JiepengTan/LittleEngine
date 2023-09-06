/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/


#include "Modules/Framework/ECS/Component.h"
#include "Modules/Framework/ECS/Actor.h"

LittleEngine::Component::~Component()
{
}

void LittleEngine::Component::SetActor(ActorPtr actor)
{
    LE_ASSERT(actor!= nullptr,"Actor should not be null!");
    if(owner != nullptr)
    {
        LOG_ERROR(" can not set comp's owner Actor again !  owner != null ");
        return;
    }
    owner = actor;
}
void LittleEngine::Component::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
    Object::OnSerialize(p_doc, p_node);
}

void LittleEngine::Component::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
    Object::OnDeserialize(p_doc, p_node);
}
