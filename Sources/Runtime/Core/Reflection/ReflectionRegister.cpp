#include <assert.h>

#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/ReflectionRegister.h"
namespace OvCore
{
    namespace Reflection
    {
        void TypeMetaRegister::MetaUnRegister() { TypeMetaRegisterInterface::UnRegisterAll(); }
    } // namespace Reflection
} // namespace LittleEngine
