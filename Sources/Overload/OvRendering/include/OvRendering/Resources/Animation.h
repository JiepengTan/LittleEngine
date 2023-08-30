#pragma once

#include <vector>
#include <map>
#include "OvRendering/Resources/AnimationData.h"
#include "OvRendering/Resources/Loaders/ModelLoader.h"

namespace OvRendering::Resources
{
    class AnimationLoader;
}


namespace OvRendering::Resources
{

    class Animation
    {
        friend class AnimationLoader;
    public:
        Animation() = default;
        ~Animation();
        BoneFrames* FindBone(const std::string& name);
        
        inline float GetTicksPerSecond() { return m_TicksPerSecond; }
        inline float GetDuration() { return m_Duration; }  

    public:
        float m_Duration;
        int m_TicksPerSecond;
        std::vector<BoneFrames> m_Bones;
        std::string m_path;

        
        std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_name2BoneInfo; }
        int& GetBoneCount() { return m_BoneCounter; }
        std::map<std::string, BoneInfo> m_name2BoneInfo;
        int m_BoneCounter = 0;
        
        SkeletonBone& GetSkeletonRoot() { return m_skeletonRoot; }
        SkeletonBone m_skeletonRoot;
    };
}
