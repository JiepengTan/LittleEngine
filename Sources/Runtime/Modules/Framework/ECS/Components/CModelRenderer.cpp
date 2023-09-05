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
#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Actor.h"

LittleEngine::CModelRenderer::CModelRenderer(Actor& p_owner) : Component(p_owner)
{
	m_modelChangedEvent += [this]
	{
		if (auto materialRenderer = owner->GetComponent<CMaterialRenderer>())
			materialRenderer->UpdateMaterialList();
	};
}

std::string LittleEngine::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void LittleEngine::CModelRenderer::SetModel(LittleEngine::Rendering::Resources::Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

LittleEngine::Rendering::Resources::Model * LittleEngine::CModelRenderer::GetModel() const
{
	return m_model;
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

void LittleEngine::CModelRenderer::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	LittleEngine::Serializer::SerializeModel(p_doc, p_node, "model", m_model);
	LittleEngine::Serializer::SerializeInt(p_doc, p_node, "frustum_behaviour", reinterpret_cast<int&>(m_frustumBehaviour));
	LittleEngine::Serializer::SerializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.position);
	LittleEngine::Serializer::SerializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.radius);
}

void LittleEngine::CModelRenderer::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode* p_node)
{
	LittleEngine::Serializer::DeserializeModel(p_doc, p_node, "model", m_model);
	LittleEngine::Serializer::DeserializeInt(p_doc, p_node, "frustum_behaviour", reinterpret_cast<int&>(m_frustumBehaviour));
	LittleEngine::Serializer::DeserializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.position);
	LittleEngine::Serializer::DeserializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.radius);
}

void LittleEngine::CModelRenderer::OnInspector(LittleEngine::UI::Internal::WidgetContainer& p_root)
{
	using namespace LittleEngine::Helpers;

	GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);

	GUIDrawer::CreateTitle(p_root, "Frustum Culling Behaviour");
	auto& boundingMode = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::ComboBox>(0);
	boundingMode.choices.emplace(0, "Disabled");
	boundingMode.choices.emplace(1, "Cull model");
	boundingMode.choices.emplace(2, "Cull model & sub-meshes");
	boundingMode.choices.emplace(3, "Cull custom bounding sphere");
	auto& boundingModeDispatcher = boundingMode.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<int>>();
	boundingModeDispatcher.RegisterReference(reinterpret_cast<int&>(m_frustumBehaviour));

	auto& centerLabel = p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Bounding Sphere Center", GUIDrawer::TitleColor);
	auto& centerWidget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GUIDrawer::GetDataType<float>(), GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT, 0.f, 0.05f, "", GUIDrawer::GetFormat<float>());
	auto& centerDispatcher = centerWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();
	centerDispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(m_customBoundingSphere.position));

	auto& radiusLabel = p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>("Bounding Sphere Radius", GUIDrawer::TitleColor);
	auto& radiusWidget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragFloat>(0.0f, GUIDrawer::_MAX_FLOAT, 0.f, 0.1f);
	auto& radiusDispatcher = radiusWidget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<float>>();
	radiusDispatcher.RegisterReference(m_customBoundingSphere.radius);

	boundingMode.ValueChangedEvent += [&](int p_choice)
	{
		centerLabel.enabled = centerWidget.enabled = radiusLabel.enabled = radiusWidget.enabled = p_choice == 3;
	};

	centerLabel.enabled = centerWidget.enabled = radiusLabel.enabled = radiusWidget.enabled = m_frustumBehaviour == EFrustumBehaviour::CULL_CUSTOM;
}
