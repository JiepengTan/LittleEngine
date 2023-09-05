#pragma once
#include "Modules/Framework/ECS/Actor.h"

namespace LittleEditor::Core
{
    class Context;
}

namespace LittleEngine
{
    class ActorUtils
    {
    public:
        static LittleEngine::Actor* CreatePlane(LittleEngine::Actor* p_parent = nullptr, const std::string& p_name = "Empty Plane");
        static LittleEngine::Actor* CreateCube(LittleEngine::Actor* p_parent = nullptr, const std::string& p_name = "Empty Cube");
        static LittleEngine::Actor* CreateSphere(LittleEngine::Actor* p_parent = nullptr, const std::string& p_name = "Empty Sphere");
        static LittleEngine::Actor* CreateEmptyActor(LittleEngine::Actor* p_parent, const std::string& p_name);
        static LittleEngine::Actor* CreateActorWithModel(const std::string& p_path,  LittleEngine::Actor* p_parent, const std::string& p_name);
    };
}
