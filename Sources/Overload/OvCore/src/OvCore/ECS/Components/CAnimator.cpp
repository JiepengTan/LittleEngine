

#include <map>
#include <vector>
#include "OvMaths/FMatrix4.h"
#include "OvCore/ECS/Components/CAnimator.h"

#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/Global/ServiceLocator.h"
#include "OvCore/ResourceManagement/AnimationManager.h"
#include "OvCore/ResourceManagement/ModelManager.h"
#include "OvRendering/Resources/Animation.h"

OvCore::ECS::Components::CAnimator::CAnimator(ECS::Actor& p_owner) :
    AComponent(p_owner), m_currentTime(0)
{
    m_curAnim = nullptr;
    m_FinalBoneMatrices.reserve(100);
}

void OvCore::ECS::Components::CAnimator::OnStart()
{
    auto mesh =  owner.GetComponent<CModelRenderer>();
    if(mesh == nullptr) return;
    auto model = mesh->GetModel();
    if(model == nullptr) return;
    auto& mgr = OvCore::Global::ServiceLocator::Get
        <OvCore::ResourceManagement::AnimationManager>();
    mgr.m_model = model;
    auto anim = mgr.GetResource(m_animPath);
    m_curAnim = anim;
    for (int i = 0; i < 100; i++)
        m_FinalBoneMatrices.push_back(OvMaths::FMatrix4::Identity);
}

void OvCore::ECS::Components::CAnimator::OnUpdate(float dt)
{
    if (m_curAnim)
    {
        m_currentTime += m_curAnim->GetTicksPerSecond() * dt;
        m_currentTime = fmod(m_currentTime, m_curAnim->GetDuration());
        CalculateBoneTransform(m_curAnim->GetSkeletonRoot(), OvMaths::FMatrix4::Identity);
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
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }
    for (int i = 0; i < node.childrenCount; i++)
        CalculateBoneTransform(node.children[i], globalTransformation);
}

std::vector<OvMaths::FMatrix4> OvCore::ECS::Components::CAnimator::GetFinalBoneMatrices()
{
    return m_FinalBoneMatrices;
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
}