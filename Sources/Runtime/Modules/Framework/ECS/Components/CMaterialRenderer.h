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
#include "Resource/Core/ResPtr.h"

#define MAX_MATERIAL_COUNT 8

namespace LittleEngine { class Actor; }

namespace LittleEngine
{
	/**
	* A component that handle a material list, necessary for model rendering
	*/
	REFLECTION_COMPONENT_TYPE(CMaterialRenderer)
	CLASS (CMaterialRenderer : public Component, WhiteListFields,CustomEditor)
	{
		REFLECTION_COMPONENT_BODY(CMaterialRenderer)
	public:

		void OnConstruction() override;
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
		* Fill the material renderer with the given material
		* @param p_material
		*/
		void FillWithMaterial(MaterialResPtr& p_material);
		void FillWithMaterial(const std::string& p_matPath);

		/**
		* Defines the material to use for the given index
		* @param p_index
		* @param p_material
		*/
		void SetMaterialAtIndex(uint8_t p_index,MaterialResPtr& p_material);
		void SetMaterialAtIndex(uint8_t p_index,const std::string& p_matPath);
		/**
		* Returns the material to use at index
		* @param p_index
		*/
		LittleEngine::Material* GetMaterialAtIndex(uint8_t p_index);

		/**
		* Remove the material at index
		* @param p_index
		*/
		void RemoveMaterialAtIndex(uint8_t p_index);
		
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
		* Returns the material
		*/
		LittleEngine::Material* GetMaterial(int idx) const;



		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector() override;
		virtual void OnBeforeSceneSave(ActorPtr p_actor) override;
		virtual void OnAfterSceneLoaded(ActorPtr p_actor) override;

	private:
		std::array<LittleEngine::UI::Widgets::AWidget*, 3>  m_materialFields[MAX_MATERIAL_COUNT];
		META(Enable)
		MaterialResPtr m_materials[MAX_MATERIAL_COUNT];
		META(Enable)
		FMatrix4 m_userMatrix;
		
	};
}
