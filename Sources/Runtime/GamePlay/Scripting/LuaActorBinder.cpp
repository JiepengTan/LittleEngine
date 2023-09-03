/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "GamePlay/Scripting/LuaActorBinder.h"

#include "GamePlay/ECS/Actor.h"

#include "GamePlay/ECS/Components/CTransform.h"
#include "GamePlay/ECS/Components/CCamera.h"
#include "GamePlay/ECS/Components/CPhysicalBox.h"
#include "GamePlay/ECS/Components/CPhysicalSphere.h"
#include "GamePlay/ECS/Components/CPhysicalCapsule.h"
#include "GamePlay/ECS/Components/CDirectionalLight.h"
#include "GamePlay/ECS/Components/CPointLight.h"
#include "GamePlay/ECS/Components/CSpotLight.h"
#include "GamePlay/ECS/Components/CAmbientBoxLight.h"
#include "GamePlay/ECS/Components/CAmbientSphereLight.h"
#include "GamePlay/ECS/Components/CModelRenderer.h"
#include "GamePlay/ECS/Components/CMaterialRenderer.h"
#include "GamePlay/ECS/Components/CAudioSource.h"
#include "GamePlay/ECS/Components/CAudioListener.h"

void OvCore::Scripting::LuaActorBinder::BindActor(sol::state & p_luaState)
{
	using namespace OvCore::ECS;
	using namespace OvCore::ECS::Components;
}
