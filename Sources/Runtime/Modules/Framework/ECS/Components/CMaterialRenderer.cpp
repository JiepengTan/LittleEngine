/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/InputFields/InputInt.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Buttons/ButtonSmall.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Plugins/DDTarget.h"

#include "Core/Tools/Utils/PathParser.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Framework/Global/ServiceLocator.h"

LittleEngine::CMaterialRenderer::CMaterialRenderer(Actor & p_owner) : Component(p_owner)
{
	m_materials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
		m_materialFields[i].fill(nullptr);

	UpdateMaterialList();
}

std::string LittleEngine::CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void LittleEngine::CMaterialRenderer::FillWithMaterial(LittleEngine::Resources::Material & p_material)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materials[i] = &p_material;
}

void LittleEngine::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, LittleEngine::Resources::Material& p_material)
{
	m_materials[p_index] = &p_material;
}

LittleEngine::Resources::Material* LittleEngine::CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index)
{
	return m_materials.at(p_index);
}

void LittleEngine::CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < m_materials.size())
	{
		m_materials[p_index] = nullptr;;
	}
}

void LittleEngine::CMaterialRenderer::RemoveMaterialByInstance(LittleEngine::Resources::Material& p_instance)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		if (m_materials[i] == &p_instance)
			m_materials[i] = nullptr;
}

void LittleEngine::CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materials[i] = nullptr;
}

const LittleEngine::FMatrix4 & LittleEngine::CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

const LittleEngine::CMaterialRenderer::MaterialList& LittleEngine::CMaterialRenderer::GetMaterials() const
{
	return m_materials;
}

void LittleEngine::CMaterialRenderer::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	tinyxml2::XMLNode* materialsNode = p_doc.NewElement("materials");
	p_node->InsertEndChild(materialsNode);

	auto modelRenderer = owner->GetComponent<CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;

	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		LittleEngine::Serializer::SerializeMaterial(p_doc, materialsNode, "material", m_materials[i]);
	}
}

void LittleEngine::CMaterialRenderer::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	tinyxml2::XMLNode* materialsRoot = p_node->FirstChildElement("materials");
	if (materialsRoot)
	{
		tinyxml2::XMLElement* currentMaterial = materialsRoot->FirstChildElement("material");

		uint8_t materialIndex = 0;

		while (currentMaterial)
		{
			if (auto material = Global::ServiceLocator::Get<ResourceManagement::MaterialManager>()[currentMaterial->GetText()])
				m_materials[materialIndex] = material;

			currentMaterial = currentMaterial->NextSiblingElement("material");
			++materialIndex;
		}
	}

	UpdateMaterialList();
}

std::array<LittleEngine::UI::Widgets::AWidget*, 3> CustomMaterialDrawer(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Resources::Material*& p_data)
{
	using namespace LittleEngine::Helpers;

	std::array<LittleEngine::UI::Widgets::AWidget*, 3> widgets;

	widgets[0] = &p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>(p_name, GUIDrawer::TitleColor);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(displayedText);

	widgets[1] = &widget;

	widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data](auto p_receivedData)
	{
		if (LittleEngine::Utils::PathParser::GetFileType(p_receivedData.first) == LittleEngine::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::MaterialManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
			}
		}
	};

	widget.lineBreak = false;

	auto & resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = GUIDrawer::ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data]
	{
		p_data = nullptr;
		widget.content = "Empty";
	};

	widgets[2] = &resetButton;

	return widgets;
}

void LittleEngine::CMaterialRenderer::OnInspector(LittleEngine::UI::Internal::WidgetContainer & p_root)
{
	using namespace LittleEngine::Helpers;

	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materialFields[i] = CustomMaterialDrawer(p_root, "Material", m_materials[i]);

	UpdateMaterialList();
}

void LittleEngine::CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = owner->GetComponent<CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			m_materialNames[materialIndex++] = materialName;
		}

		for (uint8_t i = materialIndex; i < MAX_MATERIAL_COUNT; ++i)
			m_materialNames[i] = "";
	}

	for (uint8_t i = 0; i < m_materialFields.size(); ++i)
	{
		if (m_materialFields[i][0])
		{
			bool enabled = !m_materialNames[i].empty();
			m_materialFields[i][0]->enabled = enabled;
			m_materialFields[i][1]->enabled = enabled;
			m_materialFields[i][2]->enabled = enabled;
			reinterpret_cast<LittleEngine::UI::Widgets::Texts::Text*>(m_materialFields[i][0])->content = m_materialNames[i];
		}
	}
}

void LittleEngine::CMaterialRenderer::SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value)
{
	if (p_row < 4 && p_column < 4)
		m_userMatrix.data[4 * p_row + p_column] = p_value;
}

float LittleEngine::CMaterialRenderer::GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const
{
	if (p_row < 4 && p_column < 4)
		return m_userMatrix.data[4 * p_row + p_column];
	else
		return 0.0f;
}