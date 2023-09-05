#include <assert.h>

#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/ReflectionRegister.h"

#include "_Generated/Reflection/AllReflection.h"
#include "_Generated/Serializer/AllSerializer.ipp"
namespace OvCore
{
    namespace Reflection
    {
        void TypeMetaRegister::MetaUnRegister() { TypeMetaRegisterInterface::UnRegisterAll(); }
    } // namespace Reflection
} // namespace LittleEngine
