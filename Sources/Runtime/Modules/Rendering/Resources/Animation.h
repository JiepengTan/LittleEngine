#pragma once

#include <vector>
#include <map>
#include "Modules/Rendering/Resources/AnimationData.h"
#include "Modules/Rendering/Resources/Loaders/ModelLoader.h"

namespace LittleEngine::Rendering::Resources
{
    class AnimationLoader;
}


namespace LittleEngine::Rendering::Resources
{

    class Animation
    {
        friend class AnimationLoader;
    public:
        Animation() = default;
        ~Animation() = default;
        BoneFrames* FindBone(const std::string& name);
        void AddBone(const BoneFrames& bone){m_Bones.push_back(bone);}
        
        inline int GetTicksPerSecond() { return m_TicksPerSecond; }
        inline float GetDuration() { return m_Duration; }  
        inline void SetTicksPerSecond(int p_ticks) { m_TicksPerSecond = p_ticks; }
        inline void SetDuration(float p_duration) {  m_Duration = p_duration; }  

    public:

        std::map<std::string, BoneInfo>* GetBoneInfoMap() { return m_name2BoneInfo; }
        int* GetBoneCounter() { return m_BoneCounter; }
        int GetBoneCount() { return *m_BoneCounter; }
        
        SkeletonBone& GetSkeletonRoot() { return m_skeletonRoot; }
        std::string path;
        float m_Duration;
        int m_TicksPerSecond;
    public:
        std::vector<BoneFrames> m_Bones;
        SkeletonBone m_skeletonRoot;
        
        std::map<std::string, BoneInfo>* m_name2BoneInfo;
        int* m_BoneCounter ;
    };
}
