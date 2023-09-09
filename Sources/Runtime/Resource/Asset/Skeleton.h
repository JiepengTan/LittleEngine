#pragma once
#include "map"

#include "Core/Maths/FMatrix4.h"


namespace LittleEngine::Rendering::Resources
{

    class Skeleton
    {
    public:
        std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;
		SkeletonBone m_skeletonRoot;
    };
}
