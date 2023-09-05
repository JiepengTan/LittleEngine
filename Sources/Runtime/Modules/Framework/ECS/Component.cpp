/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Component.h"
#include "Modules/Framework/ECS/Actor.h"
LittleEngine::Component::Component(Actor& p_owner) : owner(&p_owner)
{
}

LittleEngine::Component::~Component()
{

}
