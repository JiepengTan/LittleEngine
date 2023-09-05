#pragma once

#include <map>
#include <vector>

#include "CModelRenderer.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Core/Maths/FMatrix4.h"
#include "Modules/Framework/ECS/Components/AComponent.h"

namespace OvRendering::Resources
{
	class Animation;
	class Skeleton;
}
namespace OvCore::ECS::Components
{
	REFLECTION_TYPE(CAnimator)
	CLASS (CAnimator : public AComponent, WhiteListFields)
	{
		REFLECTION_BODY(CAnimator)
	public:
		CAnimator() = default;
		CAnimator(ECS::Actor& p_owner);
		~CAnimator() =default;
		std::string GetName() override;
		void OnStart() override;
		virtual void OnUpdate(float dt) override;
		void OnDestroy() override;
		void LoadAnimations();
		void UnloadAnimations();
		void UpVertexBufferCPU();
		void UpdateBonesGameObjects();
		void UpdateBoneMatrix();
		void DumpAnimationLog();
		void PlayAnimation(OvRendering::Resources::Animation* pAnimation);
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
		
		std::vector<OvMaths::FMatrix4>* GetFinalBoneMatrices();

		void ToggleBones();

	private:
		void CalculateBoneTransform(const OvRendering::Resources::SkeletonBone& node, const OvMaths::FMatrix4&  parentTransform);
		void CreateBoneActors(const OvRendering::Resources::SkeletonBone& node,  OvMaths::FMatrix4  parentTransform);
	private:
        META(Enable)
		float m_currentTime;
		bool m_showDebugBones;
		OvRendering::Resources::Animation* m_curAnim;

		std::vector<OvMaths::FMatrix4> m_finalBoneMatrices;
		// debug infos
	private:
		std::vector<Actor*> m_debugBones;
		Actor* m_debugBoneRoot; 
		std::vector<int> m_boneId;
		float boneDrawSize = 0.05f;
		
	public:
        META(Enable)
		std::string m_animPath;
		
		bool hasUpdate = false;
	};
}
