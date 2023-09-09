#pragma once
#include "Resource/Data/ResIncludeBasic.h"

namespace LittleEngine::Resources
{
    REFLECTION_TYPE(ResMetaInfo)
    CLASS(ResMetaInfo, Fields)
    {
        REFLECTION_BODY(ResMetaInfo)
    public:
        StringName m_key;
        StringName m_val;
    };
    
    REFLECTION_TYPE(ResMeta)
    CLASS(ResMeta, Fields)
    {
        REFLECTION_BODY(ResMeta)
    public:
        StringName m_guid;
        StringName m_path;
        std::vector<LittleEngine::Resources::ResMetaInfo> m_datas;
    };
}
