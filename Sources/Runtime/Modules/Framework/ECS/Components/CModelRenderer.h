/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Geometry/Vertex.h"
#include "Resource/Asset/Model.h"

#include "Modules/Framework/ECS/Component.h"
#include "Resource/Core/ResPtr.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* A ModelRenderer is necessary in combination with a MaterialRenderer to render a model in the world
	*/
	REFLECTION_COMPONENT_TYPE(CModelRenderer)
	CLASS (CModelRenderer : public Component, WhiteListFields)
	{
		REFLECTION_COMPONENT_BODY(CModelRenderer)
		friend class CMaterialRenderer;
	public:
		/**
		* Defines how the model renderer bounding sphere should be interpreted
		*/
		enum class EFrustumBehaviour
		{
			DISABLED = 0,
			CULL_MODEL = 1,
			CULL_MESHES = 2,
			CULL_CUSTOM = 3
		};

		/**
		* Constructor
		* @param p_owner
		*/
		void OnAwake() override;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines the model to use
		* @param p_model
		*/
		void SetModel(const StringText& p_model);

		/**
		* Returns the current model
		*/
		LittleEngine::Model* GetModel();

		/**
		* Sets a bounding mode
		* @param p_boundingMode
		*/
		void SetFrustumBehaviour(EFrustumBehaviour p_boundingMode);

		/**
		* Returns the current bounding mode
		*/
		EFrustumBehaviour GetFrustumBehaviour() const;

		/**
		* Returns the custom bounding sphere
		*/
		const LittleEngine::Rendering::Geometry::BoundingSphere& GetCustomBoundingSphere() const;

		/**
		* Sets the custom bounding sphere
		* @param p_boundingSphere
		*/
		void SetCustomBoundingSphere(const LittleEngine::Rendering::Geometry::BoundingSphere& p_boundingSphere);

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;
		
		virtual void OnBeforeSceneSave(ActorPtr p_actor) override;
		virtual void OnAfterSceneLoaded(ActorPtr p_actor) override;

	private:
		LittleEngine::Eventing::Event<> m_modelChangedEvent;
		META(Enable)
		ModelResPtr m_model ;
		META(Enable)
		Rendering::Geometry::BoundingSphere m_customBoundingSphere = { {}, 1.0f };
		META(Enable)
		EFrustumBehaviour m_frustumBehaviour = EFrustumBehaviour::CULL_MODEL;
	};
}
