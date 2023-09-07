/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <filesystem>

#include "Core/Tools/Utils/PathParser.h"
#include "Core/Tools/Utils/SizeConverter.h"

#include "Modules/Utils/GUIUtil.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"

#include "Modules/UI/Widgets/Visual/Separator.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/GroupCollapsable.h"
#include "Modules/UI/Widgets/Layout/NewLine.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Selection/ComboBox.h"

#include "../Editor/Panels/AssetProperties.h"
#include "../Editor/Panels/AssetView.h"
#include "../Editor/Core/EditorActions.h"

LittleEngine::Editor::Panels::AssetProperties::AssetProperties
(
	const std::string& p_title,
	bool p_opened,
	const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
) :
	PanelWindow(p_title, p_opened, p_windowSettings)
{
    m_targetChanged += [this]() { SetTarget(m_assetSelector->content); };

	CreateHeaderButtons();

    m_headerSeparator = &CreateWidget<LittleEngine::UI::Widgets::Visual::Separator>();
    m_headerSeparator->enabled = false;

    CreateAssetSelector();

    m_settings = &CreateWidget<LittleEngine::UI::Widgets::Layout::GroupCollapsable>("Settings");
	m_settingsColumns = &m_settings->CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
	m_settingsColumns->widths[0] = 150;

    m_info = &CreateWidget<LittleEngine::UI::Widgets::Layout::GroupCollapsable>("Info");
    m_infoColumns = &m_info->CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
    m_infoColumns->widths[0] = 150;

    m_settings->enabled = m_info->enabled = false;
}

void LittleEngine::Editor::Panels::AssetProperties::SetTarget(const std::string& p_path)
{
	m_resource = p_path == "" ? p_path : EDITOR_EXEC(GetResourcePath(p_path));

    if (m_assetSelector)
    {
        m_assetSelector->content = m_resource;
    }

    Refresh();
}

void LittleEngine::Editor::Panels::AssetProperties::Refresh()
{
    m_metadata.reset(new LittleEngine::Filesystem::IniFile(EDITOR_EXEC(GetRealPath(m_resource)) + ".meta"));

    CreateSettings();
    CreateInfo();

    m_applyButton->enabled = m_settings->enabled;
    m_resetButton->enabled = m_settings->enabled;
    m_revertButton->enabled = m_settings->enabled;

    switch (LittleEngine::Utils::PathParser::GetFileType(m_resource))
    {
    case LittleEngine::Utils::PathParser::EFileType::MODEL:
    case LittleEngine::Utils::PathParser::EFileType::TEXTURE:
    case LittleEngine::Utils::PathParser::EFileType::MATERIAL:
        m_previewButton->enabled = true;
        break;
    default:
        m_previewButton->enabled = false;
        break;
    }

    // Enables the header separator (And the line break) if at least one button is enabled
    m_headerSeparator->enabled = m_applyButton->enabled || m_resetButton->enabled || m_revertButton->enabled || m_previewButton->enabled;
    m_headerLineBreak->enabled = m_headerSeparator->enabled;
}

void LittleEngine::Editor::Panels::AssetProperties::Preview()
{
	auto& assetView = EDITOR_PANEL(LittleEngine::Editor::Panels::AssetView, "Asset View");

	const auto fileType = LittleEngine::Utils::PathParser::GetFileType(m_resource);

	if (fileType == LittleEngine::Utils::PathParser::EFileType::MODEL)
	{
		if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::ModelManager).GetResource(m_resource))
		{
			assetView.SetResource(resource);
		}
	}
	else if (fileType == LittleEngine::Utils::PathParser::EFileType::TEXTURE)
	{
		if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::TextureManager).GetResource(m_resource))
		{
			assetView.SetResource(resource);
		}
	}

	assetView.Open();
}

void LittleEngine::Editor::Panels::AssetProperties::CreateHeaderButtons()
{
	m_applyButton = &CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Apply");
    m_applyButton->idleBackgroundColor = { 0.0f, 0.5f, 0.0f };
    m_applyButton->enabled = false;
    m_applyButton->lineBreak = false;
    m_applyButton->ClickedEvent += std::bind(&AssetProperties::Apply, this);

	m_revertButton = &CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Revert");
	m_revertButton->idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
    m_revertButton->enabled = false;
    m_revertButton->lineBreak = false;
    m_revertButton->ClickedEvent += std::bind(&AssetProperties::SetTarget, this, m_resource);

	m_previewButton = &CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Preview");
	m_previewButton->idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
    m_previewButton->enabled = false;
	m_previewButton->lineBreak = false;
	m_previewButton->ClickedEvent += std::bind(&AssetProperties::Preview, this);

	m_resetButton = &CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Reset to default");
	m_resetButton->idleBackgroundColor = { 0.5f, 0.0f, 0.0f };
    m_resetButton->enabled = false;
    m_resetButton->lineBreak = false;
	m_resetButton->ClickedEvent += [this]
	{
		m_metadata->RemoveAll();
		CreateSettings();
	};

    m_headerLineBreak = &CreateWidget<LittleEngine::UI::Widgets::Layout::NewLine>();
    m_headerLineBreak->enabled = false;
}

void LittleEngine::Editor::Panels::AssetProperties::CreateAssetSelector()
{
    auto& columns = CreateWidget<LittleEngine::UI::Widgets::Layout::Columns<2>>();
    columns.widths[0] = 150;
	GUIUtil::m_root = &columns;
    m_assetSelector = &GUIUtil::DrawAsset("Target", m_resource, &m_targetChanged);
}

void LittleEngine::Editor::Panels::AssetProperties::CreateSettings()
{
	m_settingsColumns->RemoveAllWidgets();

	const auto fileType = LittleEngine::Utils::PathParser::GetFileType(m_resource);

    m_settings->enabled = true;

	if (fileType == LittleEngine::Utils::PathParser::EFileType::MODEL)
	{
		CreateModelSettings();
	}
	else if (fileType == LittleEngine::Utils::PathParser::EFileType::TEXTURE)
	{
		CreateTextureSettings();
	}
    else
    {
        m_settings->enabled = false;
    }
}

void LittleEngine::Editor::Panels::AssetProperties::CreateInfo()
{
    const auto realPath = EDITOR_EXEC(GetRealPath(m_resource));

    m_infoColumns->RemoveAllWidgets();

    if (std::filesystem::exists(realPath))
    {
        m_info->enabled = true;

    	GUIUtil::m_root = m_infoColumns;
        GUIUtil::CreateTitle("Path");
        m_infoColumns->CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(realPath);

        GUIUtil::CreateTitle("Size");
        const auto [size, unit] = LittleEngine::Utils::SizeConverter::ConvertToOptimalUnit(static_cast<float>(std::filesystem::file_size(realPath)), LittleEngine::Utils::SizeConverter::ESizeUnit::BYTE);
        m_infoColumns->CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(std::to_string(size) + " " + LittleEngine::Utils::SizeConverter::UnitToString(unit));

        GUIUtil::CreateTitle("Metadata");
        m_infoColumns->CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(std::filesystem::exists(realPath + ".meta") ? "Yes" : "No");
    }
    else
    {
        m_info->enabled = false;
    }
}

#define MODEL_FLAG_ENTRY(setting) GUIUtil::DrawBoolean(setting, [&]() { return m_metadata->Get<bool>(setting); }, [&](bool value) { m_metadata->Set<bool>(setting, value); })

void LittleEngine::Editor::Panels::AssetProperties::CreateModelSettings()
{
	m_metadata->Add("CALC_TANGENT_SPACE", true);
	m_metadata->Add("JOIN_IDENTICAL_VERTICES", true);
	m_metadata->Add("MAKE_LEFT_HANDED", false);
	m_metadata->Add("TRIANGULATE", true);
	m_metadata->Add("REMOVE_COMPONENT", false);
	m_metadata->Add("GEN_NORMALS", false);
	m_metadata->Add("GEN_SMOOTH_NORMALS", true);
	m_metadata->Add("SPLIT_LARGE_MESHES", false);
	m_metadata->Add("PRE_TRANSFORM_VERTICES", true);
	m_metadata->Add("LIMIT_BONE_WEIGHTS", false);
	m_metadata->Add("VALIDATE_DATA_STRUCTURE", false);
	m_metadata->Add("IMPROVE_CACHE_LOCALITY", true);
	m_metadata->Add("REMOVE_REDUNDANT_MATERIALS", false);
	m_metadata->Add("FIX_INFACING_NORMALS", false);
	m_metadata->Add("SORT_BY_PTYPE", false);
	m_metadata->Add("FIND_DEGENERATES", false);
	m_metadata->Add("FIND_INVALID_DATA", true);
	m_metadata->Add("GEN_UV_COORDS", true);
	m_metadata->Add("TRANSFORM_UV_COORDS", false);
	m_metadata->Add("FIND_INSTANCES", true);
	m_metadata->Add("OPTIMIZE_MESHES", true);
	m_metadata->Add("OPTIMIZE_GRAPH", true);
	m_metadata->Add("FLIP_UVS", false);
	m_metadata->Add("FLIP_WINDING_ORDER", false);
	m_metadata->Add("SPLIT_BY_BONE_COUNT", false);
	m_metadata->Add("DEBONE", true);
	m_metadata->Add("GLOBAL_SCALE", true);
	m_metadata->Add("EMBED_TEXTURES", false);
	m_metadata->Add("FORCE_GEN_NORMALS", false);
	m_metadata->Add("DROP_NORMALS", false);
	m_metadata->Add("GEN_BOUNDING_BOXES", false);
	
	GUIUtil::m_root = m_settingsColumns;
	MODEL_FLAG_ENTRY("CALC_TANGENT_SPACE");
	MODEL_FLAG_ENTRY("JOIN_IDENTICAL_VERTICES");
	MODEL_FLAG_ENTRY("MAKE_LEFT_HANDED");
	MODEL_FLAG_ENTRY("TRIANGULATE");
	MODEL_FLAG_ENTRY("REMOVE_COMPONENT");
	MODEL_FLAG_ENTRY("GEN_NORMALS");
	MODEL_FLAG_ENTRY("GEN_SMOOTH_NORMALS");
	MODEL_FLAG_ENTRY("SPLIT_LARGE_MESHES");
	MODEL_FLAG_ENTRY("PRE_TRANSFORM_VERTICES");
	MODEL_FLAG_ENTRY("LIMIT_BONE_WEIGHTS");
	MODEL_FLAG_ENTRY("VALIDATE_DATA_STRUCTURE");
	MODEL_FLAG_ENTRY("IMPROVE_CACHE_LOCALITY");
	MODEL_FLAG_ENTRY("REMOVE_REDUNDANT_MATERIALS");
	MODEL_FLAG_ENTRY("FIX_INFACING_NORMALS");
	MODEL_FLAG_ENTRY("SORT_BY_PTYPE");
	MODEL_FLAG_ENTRY("FIND_DEGENERATES");
	MODEL_FLAG_ENTRY("FIND_INVALID_DATA");
	MODEL_FLAG_ENTRY("GEN_UV_COORDS");
	MODEL_FLAG_ENTRY("TRANSFORM_UV_COORDS");
	MODEL_FLAG_ENTRY("FIND_INSTANCES");
	MODEL_FLAG_ENTRY("OPTIMIZE_MESHES");
	MODEL_FLAG_ENTRY("OPTIMIZE_GRAPH");
	MODEL_FLAG_ENTRY("FLIP_UVS");
	MODEL_FLAG_ENTRY("FLIP_WINDING_ORDER");
	MODEL_FLAG_ENTRY("SPLIT_BY_BONE_COUNT");
	MODEL_FLAG_ENTRY("DEBONE");
	MODEL_FLAG_ENTRY("GLOBAL_SCALE");
	MODEL_FLAG_ENTRY("EMBED_TEXTURES");
	MODEL_FLAG_ENTRY("FORCE_GEN_NORMALS");
	MODEL_FLAG_ENTRY("DROP_NORMALS");
	MODEL_FLAG_ENTRY("GEN_BOUNDING_BOXES");
};

void LittleEngine::Editor::Panels::AssetProperties::CreateTextureSettings()
{
	m_metadata->Add("MIN_FILTER", static_cast<int>(LittleEngine::Rendering::Settings::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
	m_metadata->Add("MAG_FILTER", static_cast<int>(LittleEngine::Rendering::Settings::ETextureFilteringMode::LINEAR));
	m_metadata->Add("ENABLE_MIPMAPPING", true);

    std::map<int, std::string> filteringModes
    {
        {0x2600, "NEAREST"},
        {0x2601, "LINEAR"},
        {0x2700, "NEAREST_MIPMAP_NEAREST"},
        {0x2703, "LINEAR_MIPMAP_LINEAR"},
        {0x2701, "LINEAR_MIPMAP_NEAREST"},
        {0x2702, "NEAREST_MIPMAP_LINEAR"}
    };

	GUIUtil::m_root = m_settingsColumns;
	GUIUtil::CreateTitle( "MIN_FILTER");
	auto& minFilter = m_settingsColumns->CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(m_metadata->Get<int>("MIN_FILTER"));
	minFilter.choices = filteringModes;
	minFilter.ValueChangedEvent += [this](int p_choice)
	{
		m_metadata->Set("MIN_FILTER", p_choice);
	};

	GUIUtil::CreateTitle( "MAG_FILTER");
	auto& magFilter = m_settingsColumns->CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(m_metadata->Get<int>("MAG_FILTER"));
	magFilter.choices = filteringModes;
	magFilter.ValueChangedEvent += [this](int p_choice)
	{
		m_metadata->Set("MAG_FILTER", p_choice);
	};

	GUIUtil::DrawBoolean( "ENABLE_MIPMAPPING", [&]() { return m_metadata->Get<bool>("ENABLE_MIPMAPPING"); }, [&](bool value) { m_metadata->Set<bool>("ENABLE_MIPMAPPING", value); });
}

void LittleEngine::Editor::Panels::AssetProperties::Apply()
{
	m_metadata->Rewrite();

	const auto resourcePath = EDITOR_EXEC(GetResourcePath(m_resource));
	const auto fileType = LittleEngine::Utils::PathParser::GetFileType(m_resource);

	if (fileType == LittleEngine::Utils::PathParser::EFileType::MODEL)
	{
		auto& modelManager = OVSERVICE(LittleEngine::ResourceManagement::ModelManager);
		if (modelManager.IsResourceRegistered(resourcePath))
		{
			modelManager.AResourceManager::ReloadResource(resourcePath);
		}
	}
	else if (fileType == LittleEngine::Utils::PathParser::EFileType::TEXTURE)
	{
		auto& textureManager = OVSERVICE(LittleEngine::ResourceManagement::TextureManager);
		if (textureManager.IsResourceRegistered(resourcePath))
		{
			textureManager.AResourceManager::ReloadResource(resourcePath);
		}
	}

    Refresh();
}
