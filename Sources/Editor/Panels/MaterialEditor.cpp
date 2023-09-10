/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Panels/MaterialEditor.h"
#include "../Editor/Panels/AssetView.h"

#include "../Editor/Core/EditorActions.h"

#include "Resource/Loader/MaterialLoader.h"
#include "Modules/Utils/GUIUtil.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/UI/Widgets/Layout/GroupCollapsable.h"
#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Buttons/ButtonSmall.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"

using namespace LittleEngine::UI::Panels;
using namespace LittleEngine::UI::Widgets;
using namespace LittleEngine;

void DrawHybridVec3( const std::string& p_name, LittleEngine::FVector3& p_data, float p_step, float p_min, float p_max)
{
	GUIUtil::CreateTitle( p_name);

	auto& rightSide = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& xyzWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GUIUtil::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GUIUtil::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
	xyzWidget.lineBreak = false;

	auto& rgbWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(false, LittleEngine::Color{ p_data.x, p_data.y, p_data.z });
	auto& rgbDispatcher = rgbWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::Color>>();
	rgbDispatcher.RegisterReference(reinterpret_cast<LittleEngine::Color&>(p_data));
	rgbWidget.enabled = false;
	rgbWidget.lineBreak = false;

	auto& xyzButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("XYZ");
	xyzButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	xyzButton.lineBreak = false;

	auto& rgbButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("RGB");
	rgbButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	xyzButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = true;
		rgbWidget.enabled = false;
	};

	rgbButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = false;
		rgbWidget.enabled = true;
	};
}

void DrawHybridVec4( const std::string& p_name, LittleEngine::FVector4& p_data, float p_step, float p_min, float p_max)
{
	GUIUtil::CreateTitle( p_name);

	auto& rightSide = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& xyzWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GUIUtil::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GUIUtil::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
	xyzWidget.lineBreak = false;

	auto& rgbaWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(true, LittleEngine::Color{ p_data.x, p_data.y, p_data.z, p_data.w });
	auto& rgbaDispatcher = rgbaWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::Color>>();
	rgbaDispatcher.RegisterReference(reinterpret_cast<LittleEngine::Color&>(p_data));
	rgbaWidget.enabled = false;
	rgbaWidget.lineBreak = false;

	auto& xyzwButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("XYZW");
	xyzwButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	xyzwButton.lineBreak = false;

	auto& rgbaButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("RGBA");
	rgbaButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };

	xyzwButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = true;
		rgbaWidget.enabled = false;
	};

	rgbaButton.ClickedEvent += [&]
	{
		xyzWidget.enabled = false;
		rgbaWidget.enabled = true;
	};
}

LittleEngine::Editor::Panels::MaterialEditor::MaterialEditor
(
	const std::string& p_title,
	bool p_opened,
	const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
) :
	PanelWindow(p_title, p_opened, p_windowSettings)
{
	CreateHeaderButtons();
	CreateWidget<LittleEngine::UI::Widgets::Visual::Separator>();
	CreateMaterialSelector();
	m_settings = &CreateWidget<Layout::Group>();
	CreateShaderSelector();
	CreateMaterialSettings();
	CreateShaderSettings();

	m_settings->enabled = false;
	m_shaderSettings->enabled = false;

	m_materialDroppedEvent	+= std::bind(&MaterialEditor::OnMaterialDropped, this);
	m_shaderDroppedEvent	+= std::bind(&MaterialEditor::OnShaderDropped, this);
}

void LittleEngine::Editor::Panels::MaterialEditor::Refresh()
{
	if (m_target)
		SetTarget(*m_target);
}

void LittleEngine::Editor::Panels::MaterialEditor::SetTarget(LittleEngine::Material & p_newTarget)
{
	m_target = &p_newTarget;
	m_targetMaterialText->content = m_target->path;
	OnMaterialDropped();
}

LittleEngine::Material * LittleEngine::Editor::Panels::MaterialEditor::GetTarget() const
{
	return m_target;
}

void LittleEngine::Editor::Panels::MaterialEditor::RemoveTarget()
{
	m_target = nullptr;
	m_targetMaterialText->content = "Empty";
	OnMaterialDropped();
}

void LittleEngine::Editor::Panels::MaterialEditor::Preview()
{
	auto& assetView = EDITOR_PANEL(Panels::AssetView, "Asset View");

	if (m_target)
		assetView.SetResource(m_target);

	assetView.Open();
}

void LittleEngine::Editor::Panels::MaterialEditor::Reset()
{
	if (m_target && m_shader != nullptr)
	{
		m_target->SetShader(nullptr);
		OnShaderDropped();
	}
}

void LittleEngine::Editor::Panels::MaterialEditor::OnMaterialDropped()
{
	m_settings->enabled = m_target; // Enable m_settings group if the target material is non-null

	if (m_settings->enabled)
	{
		GenerateMaterialSettingsContent();
		m_shaderText->content = m_target->GetShader() ? m_target->GetShader()->path : "Empty";
		m_shader = m_target->GetShader();
	}
	else
	{
		m_materialSettingsColumns->RemoveAllWidgets();
	}

	m_shaderSettings->enabled = false;
	m_shaderSettingsColumns->RemoveAllWidgets();

	if (m_target && m_target->GetShader())
		OnShaderDropped();
}

void LittleEngine::Editor::Panels::MaterialEditor::OnShaderDropped()
{
	m_shaderSettings->enabled = m_shader; // Enable m_shaderSettings group if the shader of the target material is non-null

	if (m_shader != m_target->GetShader())
		m_target->SetShader(m_shader);

	if (m_shaderSettings->enabled)
	{
		GenerateShaderSettingsContent();
	}
	else
	{
		m_shaderSettingsColumns->RemoveAllWidgets();
	}
}

void LittleEngine::Editor::Panels::MaterialEditor::CreateHeaderButtons()
{
	auto& saveButton = CreateWidget<Buttons::Button>("Save to file");
	saveButton.idleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	saveButton.ClickedEvent += [this]
	{
		if (m_target)
			LittleEngine::Resources::MaterialLoader::Save(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));
	};

	saveButton.lineBreak = false;

	auto& reloadButton = CreateWidget<Buttons::Button>("Reload from file");
	reloadButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	reloadButton.ClickedEvent += [this]
	{
		if (m_target)
			LittleEngine::Resources::MaterialLoader::Reload(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));

		OnMaterialDropped();
	};

	reloadButton.lineBreak = false;

	auto& previewButton = CreateWidget<Buttons::Button>("Preview");
	previewButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	previewButton.ClickedEvent += std::bind(&MaterialEditor::Preview, this);
	previewButton.lineBreak = false;

	auto& resetButton = CreateWidget<Buttons::Button>("Reset to default");
	resetButton.idleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	resetButton.ClickedEvent += std::bind(&MaterialEditor::Reset, this);
}

void LittleEngine::Editor::Panels::MaterialEditor::CreateMaterialSelector()
{
	auto& columns = CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
	columns.widths[0] = 150;
	GUIUtil::m_root = &columns;

	m_targetMaterialText = &GUIUtil::DrawMaterial( "Material", m_target, &m_materialDroppedEvent);
}

void LittleEngine::Editor::Panels::MaterialEditor::CreateShaderSelector()
{
	auto& columns = m_settings->CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
	columns.widths[0] = 150;
	GUIUtil::m_root = &columns;
	m_shaderText = &GUIUtil::DrawShader( "Shader", m_shader, &m_shaderDroppedEvent);
}

void LittleEngine::Editor::Panels::MaterialEditor::CreateMaterialSettings()
{
	m_materialSettings = &m_settings->CreateWidget<Layout::GroupCollapsable>("Material Settings");
	m_materialSettingsColumns = &m_materialSettings->CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
	m_materialSettingsColumns->widths[0] = 150;
}

void LittleEngine::Editor::Panels::MaterialEditor::CreateShaderSettings()
{
	m_shaderSettings = &m_settings->CreateWidget<Layout::GroupCollapsable>("Shader Settings");
	m_shaderSettingsColumns = &m_shaderSettings->CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
	m_shaderSettingsColumns->widths[0] = 150;
}

std::string UniformFormat(const std::string& p_string)
{
	std::string result;
	std::string formattedInput = p_string;

	if (formattedInput.rfind("u_", 0) == 0 || formattedInput.rfind("U_", 0) == 0)
	{
		formattedInput = formattedInput.substr(2);
	}

	std::string capsChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	bool first = true;
	bool capsNext = false;

	for (char c : formattedInput)
	{
		if (first || capsNext)
		{
			c = toupper(c);
			first = false;
			capsNext = false;
		}

		if (c == '_')
		{
			c = ' ';
			capsNext = true;
		}

		if (std::find(capsChars.begin(), capsChars.end(), c) != capsChars.end())
			result.push_back(' ');

		result.push_back(c);
	}

	return result;
}

void LittleEngine::Editor::Panels::MaterialEditor::GenerateShaderSettingsContent()
{
	using namespace LittleEngine::Resources;

	m_shaderSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	std::multimap<int, std::pair<std::string, ResUniformInfo*>> sortedUniformsData;

	for (auto item : m_target->GetUniformsData())
	{
		auto name = item.m_key;
		int orderID = 0;

		auto uniformData = m_target->GetShader()->GetUniformInfo(name);

		if (uniformData)
		{
			switch (uniformData->type)
			{
			case UniformType::UNIFORM_SAMPLER_2D:	orderID = 0; break;
			case UniformType::UNIFORM_FLOAT_VEC4:	orderID = 1; break;
			case UniformType::UNIFORM_FLOAT_VEC3:	orderID = 2; break;
			case UniformType::UNIFORM_FLOAT_VEC2:	orderID = 3; break;
			case UniformType::UNIFORM_FLOAT:		orderID = 4; break;
			case UniformType::UNIFORM_INT:			orderID = 5; break;
			case UniformType::UNIFORM_BOOL:			orderID = 6; break;
			}

			sortedUniformsData.emplace(orderID, std::pair{ name, &item });
		}
	}

	for (auto& [order, info] : sortedUniformsData)
	{
		auto uniformData = m_target->GetShader()->GetUniformInfo(info.first);
		
		GUIUtil::m_root = m_shaderSettingsColumns;
		auto& data = info.second->m_data;
		if (uniformData)
		{
			switch (uniformData->type)
			{
				case UniformType::UNIFORM_BOOL:			GUIUtil::DrawBoolean( UniformFormat(info.first), reinterpret_cast<bool&>(data));																	break;
				case UniformType::UNIFORM_INT:			GUIUtil::DrawScalar<int>( UniformFormat(info.first), reinterpret_cast<int&>(data));																break;
				case UniformType::UNIFORM_FLOAT:		GUIUtil::DrawScalar<float>( UniformFormat(info.first), reinterpret_cast<float&>(data), 0.01f, GUIUtil::_MIN_FLOAT, GUIUtil::_MAX_FLOAT);		break;
				case UniformType::UNIFORM_FLOAT_VEC2:	GUIUtil::DrawVec2( UniformFormat(info.first), reinterpret_cast<LittleEngine::FVector2&>(data), 0.01f, GUIUtil::_MIN_FLOAT, GUIUtil::_MAX_FLOAT);	break;
				case UniformType::UNIFORM_FLOAT_VEC3:	DrawHybridVec3( UniformFormat(info.first), reinterpret_cast<LittleEngine::FVector3&>(data), 0.01f, GUIUtil::_MIN_FLOAT, GUIUtil::_MAX_FLOAT);			break;
				case UniformType::UNIFORM_FLOAT_VEC4:	DrawHybridVec4( UniformFormat(info.first), reinterpret_cast<LittleEngine::FVector4&>(data), 0.01f, GUIUtil::_MIN_FLOAT, GUIUtil::_MAX_FLOAT);			break;
				case UniformType::UNIFORM_SAMPLER_2D:	GUIUtil::DrawTexture( UniformFormat(info.first), info.second->m_texture);																break;
			}
		}
	}
}

void LittleEngine::Editor::Panels::MaterialEditor::GenerateMaterialSettingsContent()
{
	m_materialSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty
	GUIUtil::m_root =m_materialSettingsColumns;
	GUIUtil::DrawBoolean( "Blendable", std::bind(&LittleEngine::Material::IsBlendable, m_target), std::bind(&LittleEngine::Material::SetBlendable, m_target, std::placeholders::_1));
	GUIUtil::DrawBoolean( "Back-face Culling", std::bind(&LittleEngine::Material::HasBackfaceCulling, m_target), std::bind(&LittleEngine::Material::SetBackfaceCulling, m_target, std::placeholders::_1));
	GUIUtil::DrawBoolean( "Front-face Culling", std::bind(&LittleEngine::Material::HasFrontfaceCulling, m_target), std::bind(&LittleEngine::Material::SetFrontfaceCulling, m_target, std::placeholders::_1));
	GUIUtil::DrawBoolean( "Depth Test", std::bind(&LittleEngine::Material::HasDepthTest, m_target), std::bind(&LittleEngine::Material::SetDepthTest, m_target, std::placeholders::_1));
	GUIUtil::DrawBoolean( "Depth Writing", std::bind(&LittleEngine::Material::HasDepthWriting, m_target), std::bind(&LittleEngine::Material::SetDepthWriting, m_target, std::placeholders::_1));
	GUIUtil::DrawBoolean( "Color Writing", std::bind(&LittleEngine::Material::HasColorWriting, m_target), std::bind(&LittleEngine::Material::SetColorWriting, m_target, std::placeholders::_1));
	GUIUtil::DrawScalar<int>( "GPU Instances", std::bind(&LittleEngine::Material::GetGPUInstances, m_target), std::bind(&LittleEngine::Material::SetGPUInstances, m_target, std::placeholders::_1), 1.0f, 0, 100000);
}
