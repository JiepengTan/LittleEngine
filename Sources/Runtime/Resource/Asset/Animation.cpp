#pragma once

#include "Resource/Asset/Animation.h"
#include "Resource/Asset/Model.h"
#include "vector"
#include "map"
#include "functional"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "Core/Debug/Assertion.h"
#include "Core/Maths/FMatrix4.h"

#include "assimp/anim.h"
#include "Core/Maths/FQuaternion.h"

using namespace std;
using namespace LittleEngine;
using namespace LittleEngine::Rendering;

LittleEngine::Resources::BoneFrames::BoneFrames(const std::string& name, int ID, const aiNodeAnim* channel):
    m_LocalTransform(LittleEngine::FMatrix4::Identity),
    m_Name(name),
    m_ID(ID)
{
    m_NumPositions = channel->mNumPositionKeys;

    for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = LittleEngine::FVector3(aiPosition.x, aiPosition.y, aiPosition.z) ;
        data.timeStamp = timeStamp;
        m_Positions.push_back(data);
    }

    m_NumRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = LittleEngine::FQuaternion(aiOrientation.x, aiOrientation.y, aiOrientation.z,aiOrientation.w);
        data.timeStamp = timeStamp;
        m_Rotations.push_back(data);
    }

    m_NumScalings = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = (float)channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = LittleEngine::FVector3(scale.x, scale.y, scale.z) ;
        data.timeStamp = timeStamp;
        m_Scales.push_back(data);
    }
}

void LittleEngine::Resources::BoneFrames::Update(float animationTime)
{
    LittleEngine::FMatrix4 translation = InterpolatePosition(animationTime);
    LittleEngine::FMatrix4 rotation = InterpolateRotation(animationTime);
    LittleEngine::FMatrix4 scale = InterpolateScaling(animationTime);
    m_LocalTransform = translation * rotation * scale;
}


int LittleEngine::Resources::BoneFrames::GetPositionIndex(float animationTime)
{
    for (int index = 0; index < m_NumPositions - 1; ++index)
    {
        if (animationTime < m_Positions[index + 1].timeStamp)
            return index;
    }
    return 0;
}

int LittleEngine::Resources::BoneFrames::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < m_NumRotations - 1; ++index)
    {
        if (animationTime < m_Rotations[index + 1].timeStamp)
            return index;
    }
    return 0;
}

int LittleEngine::Resources::BoneFrames::GetScaleIndex(float animationTime)
{
    for (int index = 0; index < m_NumScalings - 1; ++index)
    {
        if (animationTime < m_Scales[index + 1].timeStamp)
            return index;
    }
    return 0;
}


float LittleEngine::Resources::BoneFrames::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

LittleEngine::FMatrix4 LittleEngine::Resources::BoneFrames::InterpolatePosition(float animationTime)
{
    if (1 == m_NumPositions)
        return FMatrix4::Translate(LittleEngine::FMatrix4::Identity, m_Positions[0].position);

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
                                       m_Positions[p1Index].timeStamp, animationTime);
    FVector3 finalPosition = FVector3::Lerp(m_Positions[p0Index].position, m_Positions[p1Index].position
                                            , scaleFactor);
    return FMatrix4::Translate(FMatrix4::Identity, finalPosition);
}

LittleEngine::FMatrix4 LittleEngine::Resources::BoneFrames::InterpolateRotation(float animationTime)
{
    if (1 == m_NumRotations)
    {
        auto rotation = FQuaternion::Normalize(m_Rotations[0].orientation);
        return FQuaternion::ToMatrix4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
                                       m_Rotations[p1Index].timeStamp, animationTime);
    FQuaternion finalRotation = FQuaternion::Slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation
                                                   , scaleFactor);
    finalRotation = FQuaternion::Normalize(finalRotation);
    return FQuaternion::ToMatrix4(finalRotation);
}

LittleEngine::FMatrix4 LittleEngine::Resources::BoneFrames::InterpolateScaling(float animationTime)
{
    if (1 == m_NumScalings)
        return FMatrix4::Scale(LittleEngine::FMatrix4::Identity, m_Scales[0].scale);

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
                                       m_Scales[p1Index].timeStamp, animationTime);
    FVector3 finalScale = FVector3::Lerp(m_Scales[p0Index].scale, m_Scales[p1Index].scale
                                         , scaleFactor);
    return FMatrix4::Scale(LittleEngine::FMatrix4::Identity, finalScale);
}



LittleEngine::Resources::BoneFrames* LittleEngine::Animation::FindBone(const std::string& name)
{
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
                             [&](const Resources::BoneFrames& Bone)
                             {
                                 return Bone.GetBoneName() == name;
                             }
    );
    if (iter == m_Bones.end()) return nullptr;
    else return &(*iter);
}


