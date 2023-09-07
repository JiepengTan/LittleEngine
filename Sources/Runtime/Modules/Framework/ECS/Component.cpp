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
void LittleEngine::Component::OnSerialize(ISerializer p_serializer)
{
    Object::OnSerialize(p_serializer);
}

void LittleEngine::Component::OnDeserialize(ISerializer p_serializer)
{
    Object::OnDeserialize(p_serializer);
}
