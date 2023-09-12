#pragma once
#include "Core/Reflection/Meta/MetaMacro.h"
#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine
{
    REFLECTION_COMPONENT_TYPE(TestComp2)
    CLASS (TestComp2 : public Component, WhiteListFields)
    {
        REFLECTION_COMPONENT_BODY(TestComp2)
    public:
        void OnUpdate(float dt) override
        {
            LOG_INFO("Hello world TestComp2");
        }
        void OnInspectorGUI() override;
    };
}
