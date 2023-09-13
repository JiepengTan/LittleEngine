#pragma once

#include "map"
#include "vector"

#include "CModelRenderer.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Core/Maths/FMatrix4.h"
#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine::Resources
{
	class Skeleton;
}
namespace LittleEngine
{
	class Animation;
	REFLECTION_COMPONENT_TYPE(CAnimator)
	CLASS (CAnimator : public Component, WhiteListFields,CustomEditor)
	{
		REFLECTION_COMPONENT_BODY(CAnimator)
	public:
		void OnAwake() override;
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
		void PlayAnimation(LittleEngine::Animation* pAnimation);
		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(ISerializer p_serializer) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(ISerializer p_serializer) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;
		
		std::vector<LittleEngine::FMatrix4>* GetFinalBoneMatrices();

		void ToggleBones();

	private:
		void CalculateBoneTransform(const LittleEngine::Resources::SkeletonBone& node, const LittleEngine::FMatrix4&  parentTransform);
		void CreateBoneActors(const LittleEngine::Resources::SkeletonBone& node,  LittleEngine::FMatrix4  parentTransform);
	private:
        META(Enable)
		float m_currentTime;
		bool m_showDebugBones;
		LittleEngine::Animation* m_curAnim;

		std::vector<LittleEngine::FMatrix4> m_finalBoneMatrices;
		// debug infos
	private:
		ActorVector m_debugBones;
		ActorPtr m_debugBoneRoot; 
		std::vector<int> m_boneId;
		float boneDrawSize = 0.05f;
		
	public:
        META(Enable)
		std::string m_animPath;
		
		bool hasUpdate = false;
	};
}
