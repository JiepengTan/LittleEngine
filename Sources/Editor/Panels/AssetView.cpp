/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Core/Tools/Utils/PathParser.h"
#include "Modules/UI/Plugins/DDTarget.h"

#include "../Editor/Core/EditorRenderer.h"
#include "../Editor/Core/EditorActions.h"
#include "../Editor/Panels/AssetView.h"


LittleEngine::Editor::Panels::AssetView::AssetView
(
	const std::string& p_title,
	bool p_opened,
	const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
) : AViewControllable(p_title, p_opened, p_windowSettings)
{
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);

	m_resource = static_cast<LittleEngine::Rendering::Resources::Model*>(nullptr);
	m_image->AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (LittleEngine::Utils::PathParser::GetFileType(path))
		{
		case LittleEngine::Utils::PathParser::EFileType::MODEL:
			if (auto resource = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::ModelManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		case LittleEngine::Utils::PathParser::EFileType::TEXTURE:
			if (auto resource = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::TextureManager>().GetResource(path); resource)
				m_resource = resource;
			break;

		case LittleEngine::Utils::PathParser::EFileType::MATERIAL:
			if (auto resource = LittleEngine::Global::ServiceLocator::Get<LittleEngine::ResourceManagement::MaterialManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		}
	};
}

void LittleEngine::Editor::Panels::AssetView::_Render_Impl()
{
	PrepareCamera();

	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();

	m_fbo.Bind();

	baseRenderer.SetStencilMask(0xFF);
	baseRenderer.Clear(m_camera);
	baseRenderer.SetStencilMask(0x00);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor);

	if (auto pval = std::get_if<LittleEngine::Rendering::Resources::Model*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderModelAsset(**pval);
	
	if (auto pval = std::get_if<LittleEngine::Rendering::Resources::Texture*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderTextureAsset(**pval);
	
	if (auto pval = std::get_if<LittleEngine::Resources::Material*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderMaterialAsset(**pval);

	baseRenderer.ApplyStateMask(glState);

	m_fbo.Unbind();
}

void LittleEngine::Editor::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

LittleEngine::Editor::Panels::AssetView::ViewableResource LittleEngine::Editor::Panels::AssetView::GetResource() const
{
	return m_resource;
}
