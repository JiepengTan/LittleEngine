/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Framework/ECS/Components/AComponent.h"
#include "Modules/Framework/ECS/Actor.h"
OvCore::ECS::Components::AComponent::AComponent(ECS::Actor& p_owner) : owner(&p_owner)
{
}

OvCore::ECS::Components::AComponent::~AComponent()
{

}
