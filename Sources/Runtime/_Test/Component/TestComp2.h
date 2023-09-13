#pragma once
#include "Core/Reflection/Meta/MetaMacro.h"
#include "Modules/Framework/ECS/Component.h"
#include "Resource/Asset/UniformInfo.h"

namespace LittleEngine
{
    REFLECTION_STRUCT_TYPE(TestStruct2)
    STRUCT(TestStruct2: public ResData, Fields)
    {
        REFLECTION_STRUCT_BODY(TestStruct2)
    public:
        std::string		name2;
        UniformType		type;
        std::string		name;
        uint32_t		location;
        ResUniformInfo	defaultValue;
    };
    
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
        char _char = 'a';
        //int8_t int8_t;
        //uint8_t uint8_t;
        //int16_t int16_t;
        //uint16_t uint16_t;
        int32_t int32_t = 0;
        uint32_t uint32_t = 0;
        TestStruct2 uniform_info;
        int64_t int64_t = 0;
        uint64_t uint64_t =0;
        float _float = 1.5f;
        double _double = 2.3;
        bool _bool = true;
        std::string _string = "asdf";
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
