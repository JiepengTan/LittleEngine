#pragma once

#include <map>
#include <vector>

#include "CModelRenderer.h"
#include "OvCore/ECS/Actor.h"
#include "OvMaths/FMatrix4.h"

namespace OvRendering::Resources
{
	class Animation;
	class Skeleton;
}
namespace OvCore::ECS::Components
{
	class CAnimator: public AComponent
	{

	public:
		CAnimator(ECS::Actor& p_owner);
		virtual void OnStart() override;

		virtual	void OnUpdate(float dt) override;
		void PlayAnimation(OvRendering::Resources::Animation* pAnimation);
		void CalculateBoneTransform(const OvRendering::Resources::SkeletonBone& node, const OvMaths::FMatrix4&  parentTransform);
		std::vector<OvMaths::FMatrix4> GetFinalBoneMatrices();
		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector(OvUI::Internal::WidgetContainer& p_root) override;

		
	private:
		std::vector<OvMaths::FMatrix4> m_FinalBoneMatrices;
		OvRendering::Resources::Animation* m_curAnim;
		float m_currentTime;
	public:
		std::string m_animPath;
	};
}
