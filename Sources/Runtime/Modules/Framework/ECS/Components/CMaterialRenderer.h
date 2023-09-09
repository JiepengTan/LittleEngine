/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "array"

#include "Modules/UI/Widgets/Layout/Columns.h"

#include "Resource/Asset/Mesh.h"

#include "Resource/Asset/Material.h"
#include "Modules/Framework/ECS/Component.h"

#define MAX_MATERIAL_COUNT 8

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* A component that handle a material list, necessary for model rendering
	*/
	REFLECTION_COMPONENT_TYPE(CMaterialRenderer)
	CLASS (CMaterialRenderer : public Component, WhiteListFields)
	{
		REFLECTION_COMPONENT_BODY(CMaterialRenderer)
	public:
		using MaterialList = std::vector<LittleEngine::Resources::Material*>;
		using MaterialField = std::vector<std::array<LittleEngine::UI::Widgets::AWidget*, 3>>;

		/**
		* Constructor
		* @param p_owner
		*/
		void DoInit(ActorPtr p_owner) override;
		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Fill the material renderer with the given material
		* @param p_material
		*/
		void FillWithMaterial(LittleEngine::Resources::Material& p_material);

		/**
		* Defines the material to use for the given index
		* @param p_index
		* @param p_material
		*/
		void SetMaterialAtIndex(uint8_t p_index, LittleEngine::Resources::Material* p_material);
		void SetMaterialAtIndex(uint8_t p_index,const std::string& p_matPath);
		/**
		* Returns the material to use at index
		* @param p_index
		*/
		LittleEngine::Resources::Material* GetMaterialAtIndex(uint8_t p_index);

		/**
		* Remove the material at index
		* @param p_index
		*/
		void RemoveMaterialAtIndex(uint8_t p_index);

		/**
		* Remove the material by instance
		* @param p_instance
		*/
		void RemoveMaterialByInstance(LittleEngine::Resources::Material& p_instance);

		/**
		* Remove every materials
		*/
		void RemoveAllMaterials();

		/**
		* Update the material list by fetching model information
		*/
		void UpdateMaterialList();

		/**
		* Defines an element of the user matrix
		* @param p_row
		* @param p_column
		* @param p_value
		*/
		void SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value);

		/**
		* Returns an element of the user matrix
		* @param p_row
		* @param p_column
		*/
		float GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const;

		/**
		* Returns the user matrix
		*/
		const LittleEngine::FMatrix4& GetUserMatrix() const;

		/**
		* Returns the materials
		*/
		const MaterialList& GetMaterials() const;

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
		virtual void OnBeforeSceneSave(ActorPtr p_actor) override;
		virtual void OnAfterSceneLoaded(ActorPtr p_actor) override;

	private:
		std::vector<LittleEngine::Resources::Material*> m_materials;
		MaterialField  m_materialFields;
		META(Enable)
		std::vector<std::string> m_materialNames;
		META(Enable)
		FMatrix4 m_userMatrix;
		
	};
}
