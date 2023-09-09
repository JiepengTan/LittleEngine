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

#include "complex.h"

#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Resource/Manager/MaterialManager.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/ModelManager.h"


void LittleEngine::CMaterialRenderer::DoInit(ActorPtr p_owner)
{
	Component::DoInit(p_owner);
	m_materials.clear();
	for (auto item : m_materialFields)
	{
		item.fill(nullptr);
	}
	UpdateMaterialList();
}

std::string LittleEngine::CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void LittleEngine::CMaterialRenderer::FillWithMaterial(LittleEngine::Resources::Material & p_material)
{
	SetMaterialAtIndex(0,&p_material);
}
void LittleEngine::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index,const std::string& p_matPath)
{
	auto mat = GetGlobalService<ResourceManagement::MaterialManager>().GetResource(p_matPath);
	SetMaterialAtIndex(p_index,mat);
}
void LittleEngine::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, LittleEngine::Resources::Material* p_material)
{
	auto count = m_materials.size();
	for (int i =count ;i<=p_index;i++)
	{
		m_materials.push_back(nullptr);
		m_materialNames.push_back("");
	}
	m_materials[p_index] = p_material;
	m_materialNames[p_index] = p_material == nullptr?p_material->path:"";
	
}

LittleEngine::Resources::Material* LittleEngine::CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index)
{
	if(p_index> m_materials.size()	) return nullptr;
	return m_materials.at(p_index);
}

void LittleEngine::CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < m_materials.size())
	{
		m_materials[p_index] = nullptr;
		m_materialNames[p_index] = "";
	}
}

void LittleEngine::CMaterialRenderer::RemoveMaterialByInstance(LittleEngine::Resources::Material& p_instance)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
	{
		if (m_materials[i] == &p_instance)
		{
			m_materials[i] = nullptr;
			m_materialNames[i] = "";
		}
	}
}

void LittleEngine::CMaterialRenderer::RemoveAllMaterials()
{
	m_materials.clear();
	m_materialNames.clear();
}

const LittleEngine::FMatrix4 & LittleEngine::CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

const LittleEngine::CMaterialRenderer::MaterialList& LittleEngine::CMaterialRenderer::GetMaterials() const
{
	return m_materials;
}

void LittleEngine::CMaterialRenderer::OnBeforeSceneSave(ActorPtr p_actor)
{

}

void LittleEngine::CMaterialRenderer::OnAfterSceneLoaded(ActorPtr p_actor)
{
	m_materials.clear();
	for (auto matName : m_materialNames)
	{
		auto mat = GetGlobalService<ResourceManagement::MaterialManager>().GetResource(matName);
		m_materials.push_back(mat);
	}
	UpdateMaterialList();
}

void LittleEngine::CMaterialRenderer::OnSerialize(ISerializer p_serializer)
{
	auto modelRenderer = owner->GetComponent<CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;
	SerializeUtil::SerializeInt("material_count", elementsToSerialize);
	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		SerializeUtil::SerializeMaterial("material" + i, m_materials[i]);
	}
}

void LittleEngine::CMaterialRenderer::OnDeserialize(ISerializer p_serializer)
{
	int count = SerializeUtil::DeserializeInt("material_count");
	for (int i =0;i< count;i++)
	{
		m_materials[i] =  SerializeUtil::DeserializeMaterial("material"+i);
	}
	UpdateMaterialList();
}

std::array<LittleEngine::UI::Widgets::AWidget*, 3> CustomMaterialDrawer( const std::string& p_name, LittleEngine::Resources::Material*& p_data)
{
	std::array<LittleEngine::UI::Widgets::AWidget*, 3> widgets;

	widgets[0] = & LittleEngine::GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>(p_name, LittleEngine::GUIUtil::TitleColor);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = LittleEngine::GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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
	resetButton.idleBackgroundColor = LittleEngine::GUIUtil::ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data]
	{
		p_data = nullptr;
		widget.content = "Empty";
	};

	widgets[2] = &resetButton;

	return widgets;
}

void LittleEngine::CMaterialRenderer::OnInspector()
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materialFields[i] = CustomMaterialDrawer( "Material", m_materials[i]);

	UpdateMaterialList();
}

void LittleEngine::CMaterialRenderer::UpdateMaterialList()
{

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
