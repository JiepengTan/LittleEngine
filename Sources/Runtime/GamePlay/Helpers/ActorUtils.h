#pragma once
#include "OvCore/ECS/Actor.h"

namespace OvEditor::Core
{
    class Context;
}

namespace OvCore::ECS
{
    class ActorUtils
    {
    public:
        static OvCore::ECS::Actor* CreatePlane(OvCore::ECS::Actor* p_parent = nullptr, const std::string& p_name = "Empty Plane");
        static OvCore::ECS::Actor* CreateCube(OvCore::ECS::Actor* p_parent = nullptr, const std::string& p_name = "Empty Cube");
        static OvCore::ECS::Actor* CreateSphere(OvCore::ECS::Actor* p_parent = nullptr, const std::string& p_name = "Empty Sphere");
        static OvCore::ECS::Actor* CreateEmptyActor(OvCore::ECS::Actor* p_parent, const std::string& p_name);
        static OvCore::ECS::Actor* CreateActorWithModel(const std::string& p_path,  OvCore::ECS::Actor* p_parent, const std::string& p_name);
    };
}
