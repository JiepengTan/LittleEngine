#pragma once

#include "vector"
#include "map"
#include "Resource/Asset/AnimationData.h"
#include "Resource/Loader/ModelLoader.h"

namespace LittleEngine::Resources{class AnimationLoader;}


namespace LittleEngine
{

    class Animation
    {
        friend class Resources::AnimationLoader;
    public:
        Animation() = default;
        ~Animation() = default;
        Resources::BoneFrames* FindBone(const std::string& name);
        void AddBone(const Resources::BoneFrames& bone){m_Bones.push_back(bone);}
        
        inline int GetTicksPerSecond() { return m_TicksPerSecond; }
        inline float GetDuration() { return m_Duration; }  
        inline void SetTicksPerSecond(int p_ticks) { m_TicksPerSecond = p_ticks; }
        inline void SetDuration(float p_duration) {  m_Duration = p_duration; }  

    public:

        std::map<std::string, Resources::BoneInfo>* GetBoneInfoMap() { return m_name2BoneInfo; }
        int* GetBoneCounter() { return m_BoneCounter; }
        int GetBoneCount() { return *m_BoneCounter; }
        
        Resources::SkeletonBone& GetSkeletonRoot() { return m_skeletonRoot; }
        std::string path;
        float m_Duration;
        int m_TicksPerSecond;
    public:
        std::vector<Resources::BoneFrames> m_Bones;
        Resources::SkeletonBone m_skeletonRoot;
        
        std::map<std::string, Resources::BoneInfo>* m_name2BoneInfo;
        int* m_BoneCounter ;
    };
}
