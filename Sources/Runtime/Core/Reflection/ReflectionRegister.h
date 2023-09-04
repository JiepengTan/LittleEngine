#pragma once
// copy from https://github.com/BoomingTech/Piccolo
namespace LittleEngine
{
    namespace Reflection
    {
        class TypeMetaRegister
        {
        public:
            static void MetaRegister();
            static void MetaUnRegister();
        };
    } // namespace Reflection
} // namespace LittleEngine