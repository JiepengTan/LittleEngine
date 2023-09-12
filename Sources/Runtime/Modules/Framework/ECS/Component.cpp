/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/


#include "Modules/Framework/ECS/Component.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Utils/InspectorUtil.h"

#if DEBUG
void LittleEngine::Component::_LE_InitDebugInfo()
{
    _LE_InternalMetaTypeName = TypeUtil::GetTypeName(_LE_InternalMetaTypeID);
}
#endif
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

void LittleEngine::Component::OnInspectorGUI()
{
    //InspectorUtil::DrawDefault(this);
}
