/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <array>

#include "Modules/UI/Widgets/Layout/Columns.h"

#include "Modules/Rendering/Resources/Mesh.h"

#include "Modules/Rendering/Resources/Material.h"
#include "Modules/Framework/ECS/Component.h"

#define MAX_MATERIAL_COUNT 255

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* A component that handle a material list, necessary for model rendering
	*/
	class CMaterialRenderer : public Component
	{
	public:
		using MaterialList = std::array<LittleEngine::Resources::Material*, MAX_MATERIAL_COUNT>;
		using MaterialField = std::array<std::array<LittleEngine::UI::Widgets::AWidget*, 3>, MAX_MATERIAL_COUNT>;

		/**
		* Constructor
		* @param p_owner
		*/
		CMaterialRenderer(Actor& p_owner);
		CMaterialRenderer() = default;
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
		void SetMaterialAtIndex(uint8_t p_index, LittleEngine::Resources::Material& p_material);

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
		MaterialList m_materials;
		MaterialField m_materialFields;
		std::array<std::string, MAX_MATERIAL_COUNT> m_materialNames;
		LittleEngine::FMatrix4 m_userMatrix;
	};
}
