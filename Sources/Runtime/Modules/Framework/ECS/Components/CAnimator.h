#pragma once

#include <map>
#include <vector>

#include "CModelRenderer.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Core/Maths/FMatrix4.h"
#include "Modules/Framework/ECS/Components/AComponent.h"

namespace LittleEngine::Rendering::Resources
{
	class Animation;
	class Skeleton;
}
namespace LittleEngine
{
	REFLECTION_TYPE(CAnimator)
	CLASS (CAnimator : public AComponent, WhiteListFields)
	{
		REFLECTION_BODY(CAnimator)
	public:
		CAnimator() = default;
		CAnimator(Actor& p_owner);
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
		void PlayAnimation(LittleEngine::Rendering::Resources::Animation* pAnimation);
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
		virtual void OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root) override;
		
		std::vector<LittleEngine::FMatrix4>* GetFinalBoneMatrices();

		void ToggleBones();

	private:
		void CalculateBoneTransform(const LittleEngine::Rendering::Resources::SkeletonBone& node, const LittleEngine::FMatrix4&  parentTransform);
		void CreateBoneActors(const LittleEngine::Rendering::Resources::SkeletonBone& node,  LittleEngine::FMatrix4  parentTransform);
	private:
        META(Enable)
		float m_currentTime;
		bool m_showDebugBones;
		LittleEngine::Rendering::Resources::Animation* m_curAnim;

		std::vector<LittleEngine::FMatrix4> m_finalBoneMatrices;
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
