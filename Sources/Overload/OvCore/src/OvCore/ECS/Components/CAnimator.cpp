#include <map>
#include <vector>
#include "OvMaths/FMatrix4.h"
#include "OvCore/ECS/Components/CAnimator.h"

#include "OvAnalytics/Profiling/ProfilerSpy.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/Global/ServiceLocator.h"
#include "OvCore/Helpers/ActorUtils.h"
#include "OvCore/ResourceManagement/AnimationManager.h"
#include "OvCore/ResourceManagement/ModelManager.h"
#include "OvRendering/Resources/Animation.h"
#include "OvRendering/Resources/Model.h"
#include "OvCore/Helpers/ActorUtils.h"
#include "OvMaths/DebugUtil.h"
#include "OvUI/Widgets/Buttons/Button.h"

const int INIT_BONE_COUNT = 100;

OvCore::ECS::Components::CAnimator::CAnimator(ECS::Actor& p_owner) :
    AComponent(p_owner), m_currentTime(0), m_showDebugBones(false)
{
    m_curAnim = nullptr;
    m_finalBoneMatrices.reserve(INIT_BONE_COUNT);
}

std::string OvCore::ECS::Components::CAnimator::GetName()
{
    return "Animator";
}

void OvCore::ECS::Components::CAnimator::UnloadAnimations()
{
    for (auto bone : m_debugBones)
    {
        bone->Destroy();
    }
    m_debugBones.clear();
    m_boneId.clear();
    m_finalBoneMatrices.clear();
    m_curAnim = nullptr;
}

void OvCore::ECS::Components::CAnimator::LoadAnimations()
{
    if (m_curAnim != nullptr) return;
    //owner.transform.SetLocalScale(OvMaths::FVector3::One*0.01f);
    auto mesh = owner.GetComponent<CModelRenderer>();
    if (mesh == nullptr) return;
    auto model = mesh->GetModel();
    if (model == nullptr) return;
    auto& mgr = OvCore::Global::ServiceLocator::Get
        <OvCore::ResourceManagement::AnimationManager>();
    mgr.currentModel = model;
    auto anim = mgr.GetResource(m_animPath);
    m_curAnim = anim;
    for (int i = 0; i < m_curAnim->GetBoneCount(); i++)
        m_finalBoneMatrices.push_back(OvMaths::FMatrix4::Identity);
    if (anim && m_showDebugBones)
    {
        CreateBoneActors(m_curAnim->GetSkeletonRoot(), OvMaths::FMatrix4::Identity);
        OVLOG("Creaet Actor Done ");
    }
}

void OvCore::ECS::Components::CAnimator::OnStart()
{
    LoadAnimations();
}

void OvCore::ECS::Components::CAnimator::CreateBoneActors(const OvRendering::Resources::SkeletonBone& node,
                                                          OvMaths::FMatrix4 parentTransform)
{
    std::string nodeName = node.name;
    OvMaths::FMatrix4 globalTransformation = parentTransform * node.transformation;
    auto& boneInfoMap = *m_curAnim->GetBoneInfoMap();
    auto boneActor = ActorUtils::CreateEmptyActor(&owner, nodeName);
    m_debugBones.push_back(boneActor);
    int id = -1;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        id = boneInfoMap[nodeName].id;
        const OvMaths::FMatrix4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[id] = globalTransformation * offset;
        boneActor->transform.SetLocalMatrix(m_finalBoneMatrices[id]);
    }
    boneActor->transform.SetLocalScale(OvMaths::FVector3::One * boneDrawSize);
    m_boneId.push_back(id);
    for (int i = 0; i < node.childrenCount; i++)
        CreateBoneActors(node.children[i], globalTransformation);
}

void OvCore::ECS::Components::CAnimator::OnDestroy()
{
    UnloadAnimations();
}

void OvCore::ECS::Components::CAnimator::UpVertexBufferCPU()
{
    PROFILER_SPY("Animator::UpVertexBufferCPU");
    auto model = owner.GetComponent<CModelRenderer>()->GetModel();
    auto& boneMatrixs = m_finalBoneMatrices;
    auto& structInfos = OvRendering::Geometry::VertexStructInfos;
    auto& positionStructInfo = structInfos[(int)OvRendering::Geometry::EVertexDataFlagsIndex::position];
    auto& boneIdStructInfo = structInfos[(int)OvRendering::Geometry::EVertexDataFlagsIndex::boneId];
    auto& boneWeightStructInfo = structInfos[(int)OvRendering::Geometry::EVertexDataFlagsIndex::boneWeight];
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

            OvMaths::FVector3 rawPos;
            OvMaths::FVector3 finalPos;
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
                    if (boneId == -1) continue;
                    if (boneId >= boneCount)
                    {
                        finalPos = rawPos;
                        break;
                    }
                    finalPos += OvMaths::FMatrix4::MultiplyPoint(boneMatrixs[boneId], rawPos) * weight;
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

void OvCore::ECS::Components::CAnimator::UpdateBonesGameObjects()
{
    PROFILER_SPY("Animator::UpdateBonesGameObjects");
    if (m_showDebugBones)
    {
        for (int i = 0; i < m_boneId.size(); i++)
        {
            if (m_boneId[i] > 0)
            {
                m_debugBones[i]->transform.SetLocalMatrix(m_finalBoneMatrices[m_boneId[i]]);
                m_debugBones[i]->transform.SetLocalScale(OvMaths::FVector3::One * boneDrawSize);
            }
        }
    }
}

void OvCore::ECS::Components::CAnimator::UpdateBoneMatrix()
{
    PROFILER_SPY("Animator::UpdateBoneMatrix");
    CalculateBoneTransform(m_curAnim->GetSkeletonRoot(), OvMaths::FMatrix4::Identity);
}

void OvCore::ECS::Components::CAnimator::DumpAnimationLog()
{
    int idx = 0;
    for (auto mat : m_finalBoneMatrices)
    {
        DebugUtil::Dump(std::to_string(idx++) + " =");
        DebugUtil::Dump(mat, true);
    }
    DebugUtil::Flush();
}

void OvCore::ECS::Components::CAnimator::OnUpdate(float dt)
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

void OvCore::ECS::Components::CAnimator::PlayAnimation(OvRendering::Resources::Animation* pAnimation)
{
    m_curAnim = pAnimation;
    m_currentTime = 0.0f;
}

void OvCore::ECS::Components::CAnimator::CalculateBoneTransform(const OvRendering::Resources::SkeletonBone& node,
                                                                const OvMaths::FMatrix4& parentTransform)
{
    std::string nodeName = node.name;
    OvMaths::FMatrix4 nodeTransform = node.transformation;
    OvRendering::Resources::BoneFrames* bone = m_curAnim->FindBone(nodeName);
    if (bone)
    {
        bone->Update(m_currentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    OvMaths::FMatrix4 globalTransformation = parentTransform * nodeTransform;

    DebugUtil::Dump(" " + nodeName + " :" + (bone == nullptr ? "" : std::to_string(bone->GetBoneID())) + "=");
    DebugUtil::Dump(globalTransformation, true);

    auto& boneInfoMap = *m_curAnim->GetBoneInfoMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        const int index = boneInfoMap[nodeName].id;
        const OvMaths::FMatrix4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[index] = globalTransformation * offset;
    }
    for (int i = 0; i < node.childrenCount; i++)
        CalculateBoneTransform(node.children[i], globalTransformation);
}

std::vector<OvMaths::FMatrix4>* OvCore::ECS::Components::CAnimator::GetFinalBoneMatrices()
{
    return &m_finalBoneMatrices;
}


void OvCore::ECS::Components::CAnimator::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
    OvCore::Helpers::Serializer::SerializeString(p_doc, p_node, "animPath", m_animPath);
}

void OvCore::ECS::Components::CAnimator::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
    OvCore::Helpers::Serializer::DeserializeString(p_doc, p_node, "animPath", m_animPath);
}

void OvCore::ECS::Components::CAnimator::ToggleBones()
{
    OVLOG("ToggleBones " + std::to_string(m_curAnim == nullptr));
    if (!m_curAnim)
    {
        LoadAnimations();
    }
    else
    {
        UnloadAnimations();
    }
}

void OvCore::ECS::Components::CAnimator::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
    using namespace OvCore::Helpers;
    GUIDrawer::DrawBoolean(p_root, "IsUpdateInEdit", IsUpdateInEdit);
    GUIDrawer::DrawAsset(p_root, "animPath", m_animPath);
    GUIDrawer::DrawScalar<float>(p_root, "currentTime", m_currentTime, 0.005f, GUIDrawer::_MIN_FLOAT,
                                 GUIDrawer::_MAX_FLOAT);
    GUIDrawer::DrawBoolean(p_root, "showDebugBones", m_showDebugBones);
    GUIDrawer::DrawScalar(p_root, "boneDrawSize", boneDrawSize);

    auto& btn = p_root.CreateWidget<OvUI::Widgets::Buttons::Button>("CreateBones");
    btn.ClickedEvent += std::bind(&OvCore::ECS::Components::CAnimator::ToggleBones, this);
}
