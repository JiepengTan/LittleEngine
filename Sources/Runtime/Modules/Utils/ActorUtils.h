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
        static ActorPtr CreatePlane(LittleEngine::ActorPtr p_parent = nullptr, const std::string& p_name = "Empty Plane");
        static ActorPtr CreateCube(LittleEngine::ActorPtr p_parent = nullptr, const std::string& p_name = "Empty Cube");
        static ActorPtr CreateSphere(LittleEngine::ActorPtr p_parent = nullptr, const std::string& p_name = "Empty Sphere");
        static ActorPtr CreateEmptyActor(LittleEngine::ActorPtr p_parent, const std::string& p_name);
        static ActorPtr CreateActorWithModel(const std::string& p_path,  LittleEngine::ActorPtr p_parent, const std::string& p_name);
    };
}
