#pragma once
#include "Core/Reflection/Reflection.h"
#include "Modules/Framework/ECS/Component.h"
#include "Modules/Framework/ECS/Components/CTransform.h"
#include "Resource/Asset/UniformType.h"

namespace LittleEngine::Test
{
    REFLECTION_COMPONENT_TYPE(TestComp3)
    CLASS (TestComp3 : public LittleEngine::Component, Fields,WhiteListMethods )
    {
        REFLECTION_COMPONENT_BODY(TestComp3)
    public:

        float health = 3;
        StringName name;

        std::string OnStarts()
        {
            LOG_INFO("Hello Little Fish! name = " + name);
            LittleEngine::UniformType instance;
            switch (instance)
            {
                case UniformType::UNIFORM_BOOL: return "UNIFORM_BOOL";
                default: break;
            }
            return "unknown type  UniformType " + std::to_string((int64_t)instance) ;
        }
        float rotateSpeed = 10;
        META(OnValueChanged:PrintValue)
        float degY;
        void OnUpdate(float p_deltaTime) override
        {
            degY+= p_deltaTime * rotateSpeed;
            GetTransform()->SetLocalRotation(FVector3(45,degY,0));
        }
        
        META(Button)
        void OnClickHello(){ LOG_INFO("hello  " + std::to_string(health));}

        META(Enable)
        void PrintValue(float p_value)
        {
            LOG_INFO("OnValueChanged_degY " + std::to_string(p_value));
        }
        META(Enable)
        float PrintValueAAA(float p_value,int val,Component* info)
        {
            LOG_INFO("OnValueChanged_degY " + std::to_string(p_value));
            return 1;
        }
    };
}
