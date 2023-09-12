#pragma once
#include "Core/Reflection/Meta/MetaMacro.h"
#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine
{
    REFLECTION_COMPONENT_TYPE(TestComp2)
    CLASS (TestComp2 : public Component, WhiteListFields, CustomerEditor )
    {
        REFLECTION_COMPONENT_BODY(TestComp2)
    public:
        void OnUpdate(float dt) override
        {
            LOG_INFO("Hello world TestComp2");
        }
        void OnInspectorGUI() override;
    private:
        META(OnEnable)
        float dataFloat;
        META(OnEnable)
        FVector2 dataVec2;
        META(OnEnable)
        FVector3 dataVec3;
        META(OnEnable)
        FVector4 dataVec4;
        META(OnEnable)
        Color dataColor;
        META(OnEnable)
        TextureResPtr ptr1;
        META(OnEnable)
        ModelResPtr ptr2;
        META(OnEnable)
        SoundResPtr ptr3;
        META(OnEnable)
        MaterialResPtr  ptr4;
        META(OnEnable)
        ShaderResPtr ptr5;
    };
}
