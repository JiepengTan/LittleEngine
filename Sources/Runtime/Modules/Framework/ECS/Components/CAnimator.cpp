#include "map"
#include "vector"
#include "Core/Maths/FMatrix4.h"
#include "Modules/Framework/ECS/Components/CAnimator.h"

#include "Core/Analytics/Profiling/ProfilerSpy.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Utils/ActorUtils.h"
#include "Resource/Manager/AnimationManager.h"
#include "Resource/Manager/ModelManager.h"
#include "Resource/Asset/Animation.h"
#include "Resource/Asset/Model.h"
#include "Modules/Utils/ActorUtils.h"
#include "Core/Maths/DebugUtil.h"
#include "Modules/UI/Widgets/Buttons/Button.h"

const int INIT_BONE_COUNT = 100;

void LittleEngine::CAnimator::OnAwake()
{
    Component::OnAwake();
    m_currentTime = 0;
    m_showDebugBones = false;
    m_curAnim = nullptr;
    m_finalBoneMatrices.reserve(INIT_BONE_COUNT);
}

std::string LittleEngine::CAnimator::GetName()
{
    return "Animator";
}

void LittleEngine::CAnimator::UnloadAnimations()
{
    for (auto bone : m_debugBones)
    {
        bone->Destroy();
    }
    m_debugBones.clear();
    
    if(m_debugBoneRoot != nullptr)
    {
        m_debugBoneRoot->Destroy();
        m_debugBoneRoot = nullptr;
    }
    m_boneId.clear();
    m_finalBoneMatrices.clear();
    m_curAnim = nullptr;
}

void LittleEngine::CAnimator::LoadAnimations()
{
    if (m_curAnim != nullptr) return;
    //owner->transform->SetLocalScale(LittleEngine::FVector3::One*0.01f);
    auto mesh = GetActor()->GetComponent<CModelRenderer>();
    if (mesh == nullptr) return;
    auto model = mesh->GetModel();
    if (model == nullptr) return;
    auto& mgr = GetGlobalService
        <LittleEngine::ResourceManagement::AnimationManager>();
    mgr.currentModel = model;
    auto anim = mgr.GetResource(m_animPath);
    m_curAnim = anim;
    for (int i = 0; i < m_curAnim->GetBoneCount(); i++)
        m_finalBoneMatrices.push_back(LittleEngine::FMatrix4::Identity);
    if (anim && m_showDebugBones)
    {
        m_debugBoneRoot  = ActorUtils::CreateEmptyActor(GetActor(), "BoneRoot");
        CreateBoneActors(m_curAnim->GetSkeletonRoot(), LittleEngine::FMatrix4::Identity);
        LOG_INFO("Creaet Actor Done ");
    }
}

void LittleEngine::CAnimator::OnStart()
{
    LoadAnimations();
}

void LittleEngine::CAnimator::CreateBoneActors(const LittleEngine::Resources::SkeletonBone& node,
                                                          LittleEngine::FMatrix4 parentTransform)
{
    std::string nodeName = node.name;
    LittleEngine::FMatrix4 globalTransformation = parentTransform * node.transformation;
    auto& boneInfoMap = *m_curAnim->GetBoneInfoMap();
    auto boneActor = ActorUtils::CreateCube(m_debugBoneRoot, nodeName);
    m_debugBones.push_back(boneActor);
    int id = -1;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        id = boneInfoMap[nodeName].id;
        const LittleEngine::FMatrix4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[id] = globalTransformation * offset;
        boneActor->transform->SetLocalMatrix(m_finalBoneMatrices[id]);
    }
    boneActor->transform->SetLocalScale(LittleEngine::FVector3::One * boneDrawSize);
    m_boneId.push_back(id);
    for (int i = 0; i < node.childrenCount; i++)
        CreateBoneActors(node.children[i], globalTransformation);
}

void LittleEngine::CAnimator::OnDestroy()
{
    UnloadAnimations();
}

void LittleEngine::CAnimator::UpVertexBufferCPU()
{
    PROFILER_SPY("Animator::UpVertexBufferCPU");
    auto model = GetActor()->GetComponent<CModelRenderer>()->GetModel();
    auto& boneMatrixs = m_finalBoneMatrices;
    auto& structInfos = LittleEngine::Rendering::Geometry::VertexStructInfos;
    auto& positionStructInfo = structInfos[(int)LittleEngine::Rendering::Geometry::EVertexDataFlagsIndex::position];
    auto& boneIdStructInfo = structInfos[(int)LittleEngine::Rendering::Geometry::EVertexDataFlagsIndex::boneId];
    auto& boneWeightStructInfo = structInfos[(int)LittleEngine::Rendering::Geometry::EVertexDataFlagsIndex::boneWeight];
    auto elemSize = positionStructInfo.MemorySize();
    int boneCount = m_curAnim->GetBoneCount();
    for (auto mesh : model->GetMeshes())
    {
        if (!mesh->isSkinMesh) continue;
        float* rawPositions = mesh->GetPositions();
        float* animatedPositions = mesh->GetAnimatedPositions();
        int* boneIds = mesh->GetBoneIds();
        float* boneWeights = mesh->GetBoneWeights();
        const int vertexCount = mesh->GetVertexCount();
        // init
        memcpy(animatedPositions, rawPositions, mesh->GetPositionBufferSize());

        // animate
        for (int i = 0; i < vertexCount; i++)
        {
            int vertexOffset = i * positionStructInfo.elemCount;
            int boneIdOffset = i * boneIdStructInfo.elemCount;
            int boneWeightOffset = i * boneWeightStructInfo.elemCount;

            LittleEngine::FVector3 rawPos;
            LittleEngine::FVector3 finalPos;
            // get raw local pos
            memcpy(&rawPos, rawPositions + vertexOffset, elemSize);
            // apply animation
            bool hasAnimation = boneIds[boneIdOffset] != -1;
            if (hasAnimation)
            {
                for (int idx = 0; idx < 4; idx++)
                {
                    auto boneId = boneIds[boneIdOffset + idx];
                    auto weight = boneWeights[boneWeightOffset + idx];
                    if (boneId == -1)
                        break;
                    if (boneId >= boneCount)
                    {
                        finalPos = rawPos;
                        break;
                    }
                    finalPos += LittleEngine::FMatrix4::MultiplyPoint(boneMatrixs[boneId], rawPos) * weight;
                }
            }
            else
            {
                finalPos = rawPos;
            }
            // copy result to target buffer
            memcpy(animatedPositions + vertexOffset, &finalPos, elemSize);
        }
        // apply result to gpu
        mesh->Rebind();
    }
}

void LittleEngine::CAnimator::UpdateBonesGameObjects()
{
    PROFILER_SPY("Animator::UpdateBonesGameObjects");
    if(m_showDebugBones)
    {
        auto& boneInfoMap = *m_curAnim->GetBoneInfoMap();
        for (auto item : boneInfoMap)
        {
            int i = item.second.id;
            if(i< m_boneId.size()&& m_boneId[i]>0)
            {
                auto local2Model = LittleEngine::FMatrix4::Inverse(item.second.offset);
                auto modelPos = local2Model * LittleEngine::FVector4(0,0,0,1);
                auto finalPos = m_finalBoneMatrices[m_boneId[i]] * modelPos;
                m_debugBones[i]->transform->SetLocalPosition(LittleEngine::FVector3(finalPos.x,finalPos.y,finalPos.z));
                m_debugBones[i]->transform->SetLocalScale(LittleEngine::FVector3::One * boneDrawSize);
            }
        }
    }
}

void LittleEngine::CAnimator::UpdateBoneMatrix()
{
    PROFILER_SPY("Animator::UpdateBoneMatrix");
    CalculateBoneTransform(m_curAnim->GetSkeletonRoot(), LittleEngine::FMatrix4::Identity);
}

void LittleEngine::CAnimator::DumpAnimationLog()
{
    int idx = 0;
    for (auto mat : m_finalBoneMatrices)
    {
        DebugUtil::Dump(std::to_string(idx++) + " =");
        DebugUtil::Dump(mat, true);
    }
    DebugUtil::Flush();
}

void LittleEngine::CAnimator::OnUpdate(float dt)
{
    PROFILER_SPY("Animator");
    if (m_curAnim)
    {
        hasUpdate = true;
        m_currentTime += m_curAnim->GetTicksPerSecond() * dt;
        m_currentTime = fmod(m_currentTime, m_curAnim->GetDuration());
        UpdateBoneMatrix();
        UpVertexBufferCPU();
        UpdateBonesGameObjects();
        //DumpAnimationLog();
    }
}

void LittleEngine::CAnimator::PlayAnimation(LittleEngine::Animation* pAnimation)
{
    m_curAnim = pAnimation;
    m_currentTime = 0.0f;
}

void LittleEngine::CAnimator::CalculateBoneTransform(const LittleEngine::Resources::SkeletonBone& node,
                                                                const LittleEngine::FMatrix4& parentTransform)
{
    std::string nodeName = node.name;
    LittleEngine::FMatrix4 nodeTransform = node.transformation;
    LittleEngine::Resources::BoneFrames* bone = m_curAnim->FindBone(nodeName);
    if (bone)
    {
        bone->Update(m_currentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    LittleEngine::FMatrix4 globalTransformation = parentTransform * nodeTransform;

    DebugUtil::Dump(" " + nodeName + " :" + (bone == nullptr ? "" : std::to_string(bone->GetBoneID())) + "=");
    DebugUtil::Dump(globalTransformation, true);

    auto& boneInfoMap = *m_curAnim->GetBoneInfoMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        const int index = boneInfoMap[nodeName].id;
        const LittleEngine::FMatrix4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[index] = globalTransformation * offset;
    }
    for (int i = 0; i < node.childrenCount; i++)
        CalculateBoneTransform(node.children[i], globalTransformation);
}

std::vector<LittleEngine::FMatrix4>* LittleEngine::CAnimator::GetFinalBoneMatrices()
{
    return &m_finalBoneMatrices;
}


void LittleEngine::CAnimator::OnSerialize(ISerializer p_serializer)
{
    SerializeUtil::SerializeString("animPath", m_animPath);
}

void LittleEngine::CAnimator::OnDeserialize(ISerializer p_serializer)
{
    SerializeUtil::DeserializeString("animPath", m_animPath);
}

void LittleEngine::CAnimator::ToggleBones()
{
    LOG_INFO("ToggleBones " + std::to_string(m_curAnim == nullptr));
    if (!m_curAnim)
    {
        LoadAnimations();
    }
    else
    {
        UnloadAnimations();
    }
}

void LittleEngine::CAnimator::OnInspector()
{
    
    GUIUtil::DrawBoolean( "IsUpdateInEdit", isUpdateInEdit);
    GUIUtil::DrawAsset( "animPath", m_animPath);
    GUIUtil::DrawScalar<float>( "currentTime", m_currentTime, 0.005f, GUIUtil::_MIN_FLOAT,
                                 GUIUtil::_MAX_FLOAT);
    GUIUtil::DrawBoolean( "showDebugBones", m_showDebugBones);
    GUIUtil::DrawScalar( "boneDrawSize", boneDrawSize);

    auto& btn = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("CreateBones");
    btn.ClickedEvent += std::bind(&LittleEngine::CAnimator::ToggleBones, this);
}
