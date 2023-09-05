/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Components/AComponent.h"
#include "Modules/Framework/ECS/Actor.h"
LittleEngine::AComponent::AComponent(Actor& p_owner) : owner(&p_owner)
{
}

LittleEngine::AComponent::~AComponent()
{

}
