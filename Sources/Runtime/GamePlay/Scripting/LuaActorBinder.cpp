/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/Scripting/LuaActorBinder.h"

#include "OvCore/ECS/Actor.h"

#include "OvCore/ECS/Components/CTransform.h"
#include "OvCore/ECS/Components/CCamera.h"
#include "OvCore/ECS/Components/CPhysicalBox.h"
#include "OvCore/ECS/Components/CPhysicalSphere.h"
#include "OvCore/ECS/Components/CPhysicalCapsule.h"
#include "OvCore/ECS/Components/CDirectionalLight.h"
#include "OvCore/ECS/Components/CPointLight.h"
#include "OvCore/ECS/Components/CSpotLight.h"
#include "OvCore/ECS/Components/CAmbientBoxLight.h"
#include "OvCore/ECS/Components/CAmbientSphereLight.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"
#include "OvCore/ECS/Components/CAudioSource.h"
#include "OvCore/ECS/Components/CAudioListener.h"

void OvCore::Scripting::LuaActorBinder::BindActor(sol::state & p_luaState)
{
	using namespace OvCore::ECS;
	using namespace OvCore::ECS::Components;
}
