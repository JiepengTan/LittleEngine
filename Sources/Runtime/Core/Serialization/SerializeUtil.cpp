#include "SerializeUtil.h"

#include "XmlSerializerXX.h"

namespace LittleEngine
{
    ISerializer* SerializeUtil::m_serializer = reinterpret_cast<ISerializer*>(new ISerializer());
}
