/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Geometry/Vertex.h"
#include "Modules/Rendering/Resources/Model.h"

#include "Modules/Framework/ECS/Component.h"

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* A ModelRenderer is necessary in combination with a MaterialRenderer to render a model in the world
	*/
	REFLECTION_TYPE(CModelRenderer)
	CLASS (CModelRenderer : public Component, WhiteListFields)
	{
		REFLECTION_BODY(CModelRenderer)
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
		void DoInit(ActorPtr p_owner) override;
		CModelRenderer() = default;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Defines the model to use
		* @param p_model
		*/
		void SetModel(LittleEngine::Rendering::Resources::Model* p_model);

		/**
		* Returns the current model
		*/
		LittleEngine::Rendering::Resources::Model* GetModel() const;

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

	private:
		LittleEngine::Rendering::Resources::Model* m_model = nullptr;
		LittleEngine::Eventing::Event<> m_modelChangedEvent;
		LittleEngine::Rendering::Geometry::BoundingSphere m_customBoundingSphere = { {}, 1.0f };
		EFrustumBehaviour m_frustumBehaviour = EFrustumBehaviour::CULL_MODEL;
	};
}
