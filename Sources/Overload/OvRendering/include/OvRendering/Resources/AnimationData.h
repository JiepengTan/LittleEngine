#pragma once

#include <vector>
#include "OvMaths/FMatrix4.h"
#include "OvMaths/FQuaternion.h"


struct aiNodeAnim;

namespace OvRendering::Resources
{
    struct BoneInfo
    {
        /*id is index in finalBoneMatrices*/
        int id;
        /*offset matrix transforms vertex from model space to bone space*/
        OvMaths::FMatrix4 offset;
    };

    struct SkeletonBone
    {
        OvMaths::FMatrix4 transformation;
        std::string name;
        int childrenCount;
        std::vector<SkeletonBone> children;
    };

    
    struct KeyPosition
    {
        OvMaths::FVector3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        OvMaths::FQuaternion orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        OvMaths::FVector3 scale;
        float timeStamp;
    };

    class BoneFrames
    {
    public:
        BoneFrames(const std::string& name, int ID, const aiNodeAnim* channel);
			
	
        void Update(float animationTime);
        OvMaths::FMatrix4 GetLocalTransform() { return m_LocalTransform; }
        std::string GetBoneName() const { return m_Name; }
        int GetBoneID() { return m_ID; }
        int GetPositionIndex(float animationTime);
        int GetRotationIndex(float animationTime);
        int GetScaleIndex(float animationTime);
    private:
        float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
        OvMaths::FMatrix4 InterpolatePosition(float animationTime);
        OvMaths::FMatrix4 InterpolateRotation(float animationTime);
        OvMaths::FMatrix4 InterpolateScaling(float animationTime);

        std::vector<KeyPosition> m_Positions;
        std::vector<KeyRotation> m_Rotations;
        std::vector<KeyScale> m_Scales;
        int m_NumPositions;
        int m_NumRotations;
        int m_NumScalings;

        OvMaths::FMatrix4 m_LocalTransform;
        std::string m_Name;
        int m_ID;
    };
}


