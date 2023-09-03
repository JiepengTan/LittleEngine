﻿#pragma once
#include <map>

#include "OvMaths/FMatrix4.h"


namespace OvRendering::Resources
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