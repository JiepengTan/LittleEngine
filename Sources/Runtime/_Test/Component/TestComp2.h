#pragma once
#include "Core/Reflection/Meta/MetaMacro.h"
#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine
{
    REFLECTION_COMPONENT_TYPE(TestComp2)
    CLASS (TestComp2 : public Component, Fields,WhiteListMethods, CustomerEditor )
    {
        REFLECTION_COMPONENT_BODY(TestComp2)
    public:
        void OnUpdate(float dt) override
        {
            LOG_INFO("Hello world TestComp2"); 
        }

        META(Enable)
        void Hello()
        {
            LOG_INFO("Hello "); 
        }
        
        META(Enable)
        void HelloLittleEngine()
        {
            LOG_INFO("HelloLittleEngine "); 
        }

        
        public:
        char _char;
        //int8_t int8_t;
        //uint8_t uint8_t;
        //int16_t int16_t;
        //uint16_t uint16_t;
        int32_t int32_t;
        uint32_t uint32_t;
        int64_t int64_t;
        uint64_t uint64_t;
        float _float;
        double _double;
        bool _bool;
        std::string _string;
        FVector2 _FVector2;
        FVector3 _FVector3;
        FVector4 _FVector4;
        Color _Color;
        TextureResPtr _TextureResPtr;
        ModelResPtr _ModelResPtr;
        SoundResPtr _SoundResPtr;
        MaterialResPtr  _MaterialResPtr;
        ShaderResPtr _ShaderResPtr;
    };
}
