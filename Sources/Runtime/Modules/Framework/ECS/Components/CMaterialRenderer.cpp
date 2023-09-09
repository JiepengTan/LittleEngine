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

void LittleEngine::CMaterialRenderer::OnConstruction()
{
	for (int i =0;i< MAX_MATERIAL_COUNT;i++)
	{
		m_materials[i] = MaterialResPtr::NullPtr;
	}
	for (auto item : m_materialFields)
	{
		item.fill(nullptr);
	}
}
void LittleEngine::CMaterialRenderer::OnAwake()
{
	Component::OnAwake();
	UpdateMaterialList();
}

std::string LittleEngine::CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void LittleEngine::CMaterialRenderer::FillWithMaterial(const std::string& p_matPath)
{
	SetMaterialAtIndex(0,p_matPath);
}
void LittleEngine::CMaterialRenderer::FillWithMaterial(MaterialResPtr& p_material)
{
	SetMaterialAtIndex(0,p_material);
}
void LittleEngine::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index,const std::string& p_matPath)
{
	auto mat = ResourcesUtils::LoadMaterialResPtr(p_matPath);
	SetMaterialAtIndex(p_index,mat);
}
void LittleEngine::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, MaterialResPtr& p_material)
{
	if(p_index<0|| p_index> MAX_MATERIAL_COUNT) return;
	m_materials[p_index] = p_material;
}

LittleEngine::Material* LittleEngine::CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index)
{
	if(p_index> MAX_MATERIAL_COUNT	) return nullptr;
	return m_materials[p_index].GetPtr();
}

void LittleEngine::CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < MAX_MATERIAL_COUNT)
	{
		m_materials[p_index] = MaterialResPtr::NullPtr;
	}
}

void LittleEngine::CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
	{
		m_materials[i] = MaterialResPtr::NullPtr;
	}
}

const LittleEngine::FMatrix4 & LittleEngine::CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

LittleEngine::Material* LittleEngine::CMaterialRenderer::GetMaterial(int idx) const
{
	if(idx >=0 && idx < MAX_MATERIAL_COUNT)
		return m_materials[idx].GetPtr();
	return nullptr;
}
void LittleEngine::CMaterialRenderer::OnBeforeSceneSave(ActorPtr p_actor)
{

}

void LittleEngine::CMaterialRenderer::OnAfterSceneLoaded(ActorPtr p_actor)
{
}


std::array<LittleEngine::UI::Widgets::AWidget*, 3> CustomMaterialDrawer( const std::string& p_name, LittleEngine::Material*& p_data)
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
	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
		m_materialFields[i] = CustomMaterialDrawer( "Material", m_materials[i].GetPtrReference());

	UpdateMaterialList();
}

void LittleEngine::CMaterialRenderer::UpdateMaterialList()
{
	for (uint8_t i = 0; i <MAX_MATERIAL_COUNT; ++i)
	{
		if (m_materialFields[i][0])
		{
			bool enabled = !m_materials[i].IsNull();
			m_materialFields[i][0]->enabled = enabled;
			m_materialFields[i][1]->enabled = enabled;
			m_materialFields[i][2]->enabled = enabled;
			reinterpret_cast<LittleEngine::UI::Widgets::Texts::Text*>(m_materialFields[i][0])->content = m_materials[i].GetPath();
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
