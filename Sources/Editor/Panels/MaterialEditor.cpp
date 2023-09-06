/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "../Editor/Panels/MaterialEditor.h"
#include "../Editor/Panels/AssetView.h"

#include "../Editor/Core/EditorActions.h"

#include "Modules/Rendering/Resources/Loaders/MaterialLoader.h"
#include "Modules/Utils/GUIDrawer.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/UI/Widgets/Layout/GroupCollapsable.h"
#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Buttons/ButtonSmall.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"

using namespace LittleEngine::UI::Panels;
using namespace LittleEngine::UI::Widgets;
using namespace LittleEngine::Helpers;

void DrawHybridVec3(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::FVector3& p_data, float p_step, float p_min, float p_max)
{
	LittleEngine::Helpers::GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& xyzWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(LittleEngine::Helpers::GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", LittleEngine::Helpers::GUIDrawer::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
	xyzWidget.lineBreak = false;

	auto& rgbWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(false, LittleEngine::UI::Types::Color{ p_data.x, p_data.y, p_data.z });
	auto& rgbDispatcher = rgbWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::UI::Types::Color>>();
	rgbDispatcher.RegisterReference(reinterpret_cast<LittleEngine::UI::Types::Color&>(p_data));
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

void DrawHybridVec4(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::FVector4& p_data, float p_step, float p_min, float p_max)
{
	LittleEngine::Helpers::GUIDrawer::CreateTitle(p_root, p_name);

	auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& xyzWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(LittleEngine::Helpers::GUIDrawer::GetDataType<float>(), p_min, p_max, 0.f, p_step, "", LittleEngine::Helpers::GUIDrawer::GetFormat<float>());
	auto& xyzDispatcher = xyzWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	xyzDispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
	xyzWidget.lineBreak = false;

	auto& rgbaWidget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(true, LittleEngine::UI::Types::Color{ p_data.x, p_data.y, p_data.z, p_data.w });
	auto& rgbaDispatcher = rgbaWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::UI::Types::Color>>();
	rgbaDispatcher.RegisterReference(reinterpret_cast<LittleEngine::UI::Types::Color&>(p_data));
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

void LittleEngine::Editor::Panels::MaterialEditor::SetTarget(LittleEngine::Resources::Material & p_newTarget)
{
	m_target = &p_newTarget;
	m_targetMaterialText->content = m_target->path;
	OnMaterialDropped();
}

LittleEngine::Resources::Material * LittleEngine::Editor::Panels::MaterialEditor::GetTarget() const
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
	if (m_target && m_shader)
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
			LittleEngine::Resources::Loaders::MaterialLoader::Save(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));
	};

	saveButton.lineBreak = false;

	auto& reloadButton = CreateWidget<Buttons::Button>("Reload from file");
	reloadButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
	reloadButton.ClickedEvent += [this]
	{
		if (m_target)
			LittleEngine::Resources::Loaders::MaterialLoader::Reload(*m_target, EDITOR_EXEC(GetRealPath(m_target->path)));

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
	m_targetMaterialText = &GUIDrawer::DrawMaterial(columns, "Material", m_target, &m_materialDroppedEvent);
}

void LittleEngine::Editor::Panels::MaterialEditor::CreateShaderSelector()
{
	auto& columns = m_settings->CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
	columns.widths[0] = 150;
	m_shaderText = &GUIDrawer::DrawShader(columns, "Shader", m_shader, &m_shaderDroppedEvent);
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
	using namespace LittleEngine::Rendering::Resources;

	m_shaderSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	std::multimap<int, std::pair<std::string, std::any*>> sortedUniformsData;

	for (auto&[name, value] : m_target->GetUniformsData())
	{
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

			sortedUniformsData.emplace(orderID, std::pair<std::string, std::any*>{ name, & value });
		}
	}

	for (auto& [order, info] : sortedUniformsData)
	{
		auto uniformData = m_target->GetShader()->GetUniformInfo(info.first);
		
		if (uniformData)
		{
			switch (uniformData->type)
			{
			case UniformType::UNIFORM_BOOL:			GUIDrawer::DrawBoolean(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<bool&>(*info.second));																	break;
			case UniformType::UNIFORM_INT:			GUIDrawer::DrawScalar<int>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<int&>(*info.second));																break;
			case UniformType::UNIFORM_FLOAT:		GUIDrawer::DrawScalar<float>(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<float&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);		break;
			case UniformType::UNIFORM_FLOAT_VEC2:	GUIDrawer::DrawVec2(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<LittleEngine::FVector2&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);	break;
			case UniformType::UNIFORM_FLOAT_VEC3:	DrawHybridVec3(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<LittleEngine::FVector3&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);			break;
			case UniformType::UNIFORM_FLOAT_VEC4:	DrawHybridVec4(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<LittleEngine::FVector4&>(*info.second), 0.01f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);			break;
			case UniformType::UNIFORM_SAMPLER_2D:	GUIDrawer::DrawTexture(*m_shaderSettingsColumns, UniformFormat(info.first), reinterpret_cast<Texture * &>(*info.second));																break;
			}
		}
	}
}

void LittleEngine::Editor::Panels::MaterialEditor::GenerateMaterialSettingsContent()
{
	m_materialSettingsColumns->RemoveAllWidgets(); // Ensure that the m_shaderSettingsColumns is empty

	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Blendable", std::bind(&LittleEngine::Resources::Material::IsBlendable, m_target), std::bind(&LittleEngine::Resources::Material::SetBlendable, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Back-face Culling", std::bind(&LittleEngine::Resources::Material::HasBackfaceCulling, m_target), std::bind(&LittleEngine::Resources::Material::SetBackfaceCulling, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Front-face Culling", std::bind(&LittleEngine::Resources::Material::HasFrontfaceCulling, m_target), std::bind(&LittleEngine::Resources::Material::SetFrontfaceCulling, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Test", std::bind(&LittleEngine::Resources::Material::HasDepthTest, m_target), std::bind(&LittleEngine::Resources::Material::SetDepthTest, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Depth Writing", std::bind(&LittleEngine::Resources::Material::HasDepthWriting, m_target), std::bind(&LittleEngine::Resources::Material::SetDepthWriting, m_target, std::placeholders::_1));
	GUIDrawer::DrawBoolean(*m_materialSettingsColumns, "Color Writing", std::bind(&LittleEngine::Resources::Material::HasColorWriting, m_target), std::bind(&LittleEngine::Resources::Material::SetColorWriting, m_target, std::placeholders::_1));
	GUIDrawer::DrawScalar<int>(*m_materialSettingsColumns, "GPU Instances", std::bind(&LittleEngine::Resources::Material::GetGPUInstances, m_target), std::bind(&LittleEngine::Resources::Material::SetGPUInstances, m_target, std::placeholders::_1), 1.0f, 0, 100000);
}
