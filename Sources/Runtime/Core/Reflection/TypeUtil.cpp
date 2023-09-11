#include "TypeUtil.h"

namespace LittleEngine
{
    TMap<TypeID, TVector<Reflection::TypeMeta*> > TypeUtil::GetBaseClassInfos()
    {
        return Reflection::TypeMeta::GetBaseClassInfos();
    }

    TVector<Reflection::TypeMeta*>  TypeUtil::GetAllTypes()
    {
        return Reflection::TypeMeta::GetAllTypes();
    }
}

