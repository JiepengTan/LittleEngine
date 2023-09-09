/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Plugins/DDTarget.h"
#include "Modules/UI/Widgets/Visual/Image.h"
#include "Modules/UI/Widgets/Selection/ComboBox.h"
#include "Modules/UI/Widgets/Selection/CheckBox.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/TextureManager.h"
#include "Resource/Manager/ModelManager.h"
#include "Resource/Manager/ShaderManager.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Actor.h"


void LittleEngine::CModelRenderer::DoInit(ActorPtr p_owner)
{
	Component::DoInit(p_owner);
	m_modelChangedEvent += [this]
	{
		if (auto materialRenderer = GetActor()->GetComponent<CMaterialRenderer>())
			materialRenderer->UpdateMaterialList();
	};
}

std::string LittleEngine::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void LittleEngine::CModelRenderer::SetModel(const StringText& p_model)
{
	m_model = ResourcesUtils::LoadModelResPtr(p_model);
	m_modelChangedEvent.Invoke();
}

LittleEngine::Model * LittleEngine::CModelRenderer::GetModel()
{
	return m_model.GetPtr();
}

void LittleEngine::CModelRenderer::SetFrustumBehaviour(EFrustumBehaviour p_boundingMode)
{
	m_frustumBehaviour = p_boundingMode;
}

LittleEngine::CModelRenderer::EFrustumBehaviour LittleEngine::CModelRenderer::GetFrustumBehaviour() const
{
	return m_frustumBehaviour;
}

const LittleEngine::Rendering::Geometry::BoundingSphere& LittleEngine::CModelRenderer::GetCustomBoundingSphere() const
{
	return m_customBoundingSphere;
}

void LittleEngine::CModelRenderer::SetCustomBoundingSphere(const LittleEngine::Rendering::Geometry::BoundingSphere& p_boundingSphere)
{
	m_customBoundingSphere = p_boundingSphere;
}



void LittleEngine::CModelRenderer::OnInspector()
{
	
	GUIUtil::DrawMesh( "Model", m_model.GetPtrReference(), &m_modelChangedEvent);

	GUIUtil::CreateTitle( "Frustum Culling Behaviour");
	auto& boundingMode = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(0);
	boundingMode.choices.emplace(0, "Disabled");
	boundingMode.choices.emplace(1, "Cull model");
	boundingMode.choices.emplace(2, "Cull model & sub-meshes");
	boundingMode.choices.emplace(3, "Cull custom bounding sphere");
	auto& boundingModeDispatcher = boundingMode.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<int>>();
	boundingModeDispatcher.RegisterReference(reinterpret_cast<int&>(m_frustumBehaviour));

	auto& centerLabel = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Bounding Sphere Center", GUIUtil::TitleColor);
	auto& centerWidget = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GUIUtil::GetDataType<float>(), GUIUtil::_MIN_FLOAT, GUIUtil::_MAX_FLOAT, 0.f, 0.05f, "", GUIUtil::GetFormat<float>());
	auto& centerDispatcher = centerWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();
	centerDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(m_customBoundingSphere.position));

	auto& radiusLabel = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Bounding Sphere Radius", GUIUtil::TitleColor);
	auto& radiusWidget = GUIUtil::CreateWidget<LittleEngine::UI::Widgets::Drags::DragFloat>(0.0f, GUIUtil::_MAX_FLOAT, 0.f, 0.1f);
	auto& radiusDispatcher = radiusWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<float>>();
	radiusDispatcher.RegisterReference(m_customBoundingSphere.radius);

	boundingMode.ValueChangedEvent += [&](int p_choice)
	{
		centerLabel.enabled = centerWidget.enabled = radiusLabel.enabled = radiusWidget.enabled = p_choice == 3;
	};

	centerLabel.enabled = centerWidget.enabled = radiusLabel.enabled = radiusWidget.enabled = m_frustumBehaviour == EFrustumBehaviour::CULL_CUSTOM;
}

void LittleEngine::CModelRenderer::OnBeforeSceneSave(ActorPtr p_actor)
{}

void LittleEngine::CModelRenderer::OnAfterSceneLoaded(ActorPtr p_actor)
{
}
