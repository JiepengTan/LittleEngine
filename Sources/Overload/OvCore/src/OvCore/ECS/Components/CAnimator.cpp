

#include <map>
#include <vector>
#include "OvMaths/FMatrix4.h"
#include "OvCore/ECS/Components/CAnimator.h"

#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/Global/ServiceLocator.h"
#include "OvCore/Helpers/ActorUtils.h"
#include "OvCore/ResourceManagement/AnimationManager.h"
#include "OvCore/ResourceManagement/ModelManager.h"
#include "OvRendering/Resources/Animation.h"
#include "OvRendering/Resources/Model.h"
#include "OvCore/Helpers/ActorUtils.h"

OvCore::ECS::Components::CAnimator::CAnimator(ECS::Actor& p_owner) :
    AComponent(p_owner), m_currentTime(0), m_showDebugBones(false)
{
    m_curAnim = nullptr;
    m_finalBoneMatrices.reserve(100);
}

std::string OvCore::ECS::Components::CAnimator::GetName()
{
    return "Animator";
}
void OvCore::ECS::Components::CAnimator::OnStart()
{
    auto mesh =  owner.GetComponent<CModelRenderer>();
    if(mesh == nullptr) return;
    auto model = mesh->GetModel();
    if(model == nullptr) return;
    auto& mgr = OvCore::Global::ServiceLocator::Get
        <OvCore::ResourceManagement::AnimationManager>();
    mgr.currentModel = model;
    auto anim = mgr.GetResource(m_animPath);
    m_curAnim = anim;
    if(anim != nullptr)
    {
        model->SetSkinMesh(true);
    }
    for (int i = 0; i < m_curAnim->m_BoneCounter; i++)
        m_finalBoneMatrices.push_back(OvMaths::FMatrix4::Identity);
    if(anim && m_showDebugBones)
    {
        CreateBoneActors(m_curAnim->GetSkeletonRoot(),OvMaths::FMatrix4::Identity);
        OVLOG("Creaet Actor Done ");
    }
}
void OvCore::ECS::Components::CAnimator::CreateBoneActors(const OvRendering::Resources::SkeletonBone& node,
                                                                 OvMaths::FMatrix4 parentTransform)
{
    
    std::string nodeName = node.name;
    OvMaths::FMatrix4 globalTransformation = parentTransform * node.transformation;
    auto boneInfoMap = m_curAnim->GetBoneInfoMap();
    auto actor = ActorUtils::CreateCube(&owner,nodeName);
    m_debugBones.push_back(actor);
    int id = -1;
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        id = boneInfoMap[nodeName].id;
        const OvMaths::FMatrix4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[id] = globalTransformation * offset;
        actor->transform.SetLocalMatrix(m_finalBoneMatrices[id]);
    }
    m_boneId.push_back(id);
    for (int i = 0; i < node.childrenCount; i++)
        CreateBoneActors(node.children[i], globalTransformation);
}

void OvCore::ECS::Components::CAnimator::OnDestroy() 
{
    for (auto bone : m_debugBones)
    {
        bone->Destroy();
    }
    m_debugBones.clear();
}
void OvCore::ECS::Components::CAnimator::OnUpdate(float dt)
{
    if (m_curAnim)
    {
        m_currentTime += m_curAnim->GetTicksPerSecond() * dt;
        m_currentTime = fmod(m_currentTime, m_curAnim->GetDuration());
        CalculateBoneTransform(m_curAnim->GetSkeletonRoot(), OvMaths::FMatrix4::Identity);
        if(m_showDebugBones)
        {
            for (int i =0;i < m_boneId.size();i++)
            {
                if(m_boneId[i]>0)
                {
                    m_debugBones[i]->transform.SetLocalMatrix(m_finalBoneMatrices[m_boneId[i]]);
                }
            }
        }
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

    auto boneInfoMap = m_curAnim->GetBoneInfoMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        const int index = boneInfoMap[nodeName].id;
        const OvMaths::FMatrix4 offset = boneInfoMap[nodeName].offset;
        m_finalBoneMatrices[index] = globalTransformation * offset;
    }
    for (int i = 0; i < node.childrenCount; i++)
        CalculateBoneTransform(node.children[i], globalTransformation);
}

std::vector<OvMaths::FMatrix4> OvCore::ECS::Components::CAnimator::GetFinalBoneMatrices()
{
    return m_finalBoneMatrices;
}


void OvCore::ECS::Components::CAnimator::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
    OvCore::Helpers::Serializer::SerializeString(p_doc, p_node, "animPath", m_animPath);
}

void OvCore::ECS::Components::CAnimator::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
    OvCore::Helpers::Serializer::DeserializeString(p_doc, p_node, "animPath", m_animPath);
}
void OvCore::ECS::Components::CAnimator::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
    using namespace OvCore::Helpers;
    GUIDrawer::DrawAsset(p_root, "animPath", m_animPath);
    GUIDrawer::DrawScalar<float>(p_root, "currentTime", m_currentTime, 0.005f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);
    
    GUIDrawer::DrawBoolean(p_root, "showDebugBones", m_showDebugBones);
}