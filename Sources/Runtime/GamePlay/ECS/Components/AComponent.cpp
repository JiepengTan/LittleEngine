/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "GamePlay/ECS/Components/AComponent.h"
#include "GamePlay/ECS/Actor.h"
OvCore::ECS::Components::AComponent::AComponent(ECS::Actor& p_owner) : owner(&p_owner)
{
}

OvCore::ECS::Components::AComponent::~AComponent()
{

}
