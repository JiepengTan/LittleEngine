#include <assert.h>

#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/ReflectionRegister.h"
#include "Core/Serialization/Json.h"
#include "Core/Serialization/JsonSerializer.h"

#include "_Generated/AllSerializer.h"
#include "_Generated/AllReflection.h"

namespace LittleEngine
{
    namespace Reflection
    {
        void TypeMetaRegister::MetaUnRegister() { TypeMetaRegisterInterface::UnRegisterAll(); }
    } // namespace Reflection
} // namespace LittleEngine
