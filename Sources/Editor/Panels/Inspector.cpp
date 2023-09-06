/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <map>

#include "../Editor/Panels/Inspector.h"

#include "Modules/UI/Widgets/Texts/Text.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Drags/DragMultipleFloats.h"
#include "Modules/UI/Widgets/Drags/DragFloat.h"
#include "Modules/UI/Widgets/Layout/Spacing.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/UI/Widgets/Visual/Image.h"
#include "Modules/UI/Widgets/InputFields/InputFloat.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"
#include "Modules/UI/Plugins/DDTarget.h"

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/ECS/Components/CTransform.h"
#include "Modules/Framework/ECS/Components/CModelRenderer.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/ECS/Components/CPhysicalBox.h"
#include "Modules/Framework/ECS/Components/CPhysicalSphere.h"
#include "Modules/Framework/ECS/Components/CPhysicalCapsule.h"
#include "Modules/Framework/ECS/Components/CPointLight.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CSpotLight.h"
#include "Modules/Framework/ECS/Components/CAmbientBoxLight.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/ECS/Components/CAudioSource.h"
#include "Modules/Framework/ECS/Components/CAudioListener.h"
#include "Modules/Framework/ECS/Components/CAnimator.h"

#include "Platform/Windowing/Dialogs/MessageBox.h"

#include "Modules/Framework/ECS/Components/CAnimator.h"
#include "../Editor/Core/EditorActions.h"

using namespace LittleEngine;
using namespace UI::Widgets;

namespace LittleEngine::Editor
{
	
Panels::Inspector::Inspector
(
	const std::string& p_title,
	bool p_opened,
	const UI::Settings::PanelWindowSettings & p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	m_inspectorHeader = &CreateWidget<UI::Widgets::Layout::Group>();
	m_inspectorHeader->enabled = false;
	m_actorInfo = &CreateWidget<UI::Widgets::Layout::Group>();

	auto& headerColumns = m_inspectorHeader->CreateWidget<UI::Widgets::Layout::Columns<2>>();

	/* Name field */
	auto nameGatherer = [this] { return m_targetActor ? m_targetActor->GetName() : "%undef%"; };
	auto nameProvider = [this](const std::string& p_newName) { if (m_targetActor) m_targetActor->SetName(p_newName); };
	Helpers::GUIDrawer::DrawString(headerColumns, "Name", nameGatherer, nameProvider);

	/* Tag field */
	auto tagGatherer = [this] { return m_targetActor ? m_targetActor->GetTag() : "%undef%"; };
	auto tagProvider = [this](const std::string & p_newName) { if (m_targetActor) m_targetActor->SetTag(p_newName); };
	Helpers::GUIDrawer::DrawString(headerColumns, "Tag", tagGatherer, tagProvider);

	/* Active field */
	auto activeGatherer = [this] { return m_targetActor ? m_targetActor->IsSelfActive() : false; };
	auto activeProvider = [this](bool p_active) { if (m_targetActor) m_targetActor->SetActive(p_active); };
	Helpers::GUIDrawer::DrawBoolean(headerColumns, "Active", activeGatherer, activeProvider);

	/* Component select + button */
	{
		auto& componentSelectorWidget = m_inspectorHeader->CreateWidget<UI::Widgets::Selection::ComboBox>(0);
		componentSelectorWidget.lineBreak = false;
		componentSelectorWidget.choices.emplace(0, "Model Renderer");
		componentSelectorWidget.choices.emplace(1, "Camera");
		componentSelectorWidget.choices.emplace(2, "Physical Box");
		componentSelectorWidget.choices.emplace(3, "Physical Sphere");
		componentSelectorWidget.choices.emplace(4, "Physical Capsule");
		componentSelectorWidget.choices.emplace(5, "Point Light");
		componentSelectorWidget.choices.emplace(6, "Directional Light");
		componentSelectorWidget.choices.emplace(7, "Spot Light");
		componentSelectorWidget.choices.emplace(8, "Ambient Box Light");
		componentSelectorWidget.choices.emplace(9, "Ambient Sphere Light");
		componentSelectorWidget.choices.emplace(10, "Material Renderer");
		componentSelectorWidget.choices.emplace(11, "Audio Source");
		componentSelectorWidget.choices.emplace(12, "Audio Listener");
		componentSelectorWidget.choices.emplace(13, "CAnimator");

		auto& addComponentButton = m_inspectorHeader->CreateWidget<UI::Widgets::Buttons::Button>("Add Component", FVector2{ 100.f, 0 });
		addComponentButton.idleBackgroundColor = UI::Types::Color{ 0.7f, 0.5f, 0.f };
		addComponentButton.textColor = UI::Types::Color::White;
		addComponentButton.ClickedEvent += [&componentSelectorWidget, this]
		{
			switch (componentSelectorWidget.currentChoice)
			{
			case 0: GetTargetActor()->AddComponent<CModelRenderer>(); GetTargetActor()->AddComponent<CMaterialRenderer>(); break;
			case 1: GetTargetActor()->AddComponent<CCamera>();				break;
			case 2: GetTargetActor()->AddComponent<CPhysicalBox>();			break;
			case 3: GetTargetActor()->AddComponent<CPhysicalSphere>();		break;
			case 4: GetTargetActor()->AddComponent<CPhysicalCapsule>();		break;
			case 5: GetTargetActor()->AddComponent<CPointLight>();			break;
			case 6: GetTargetActor()->AddComponent<CDirectionalLight>();	break;
			case 7: GetTargetActor()->AddComponent<CSpotLight>();			break;
			case 8: GetTargetActor()->AddComponent<CAmbientBoxLight>();		break;
			case 9: GetTargetActor()->AddComponent<CAmbientSphereLight>();	break;
			case 10: GetTargetActor()->AddComponent<CMaterialRenderer>();	break;
			case 11: GetTargetActor()->AddComponent<CAudioSource>();		break;
			case 12: GetTargetActor()->AddComponent<CAudioListener>();		break;
			case 13: GetTargetActor()->AddComponent<CAnimator>();		break;
			}

			componentSelectorWidget.ValueChangedEvent.Invoke(componentSelectorWidget.currentChoice);
		};

		componentSelectorWidget.ValueChangedEvent += [this, &addComponentButton](int p_value)
		{
			auto defineButtonsStates = [&addComponentButton](bool p_componentExists)
			{
				addComponentButton.disabled = p_componentExists;
				addComponentButton.idleBackgroundColor = !p_componentExists ? UI::Types::Color{ 0.7f, 0.5f, 0.f } : UI::Types::Color{ 0.1f, 0.1f, 0.1f };
			};

			switch (p_value)
			{
			case 0: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CModelRenderer>());		return;
			case 1: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CCamera>());				return;
			case 2: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CPhysicalObject>());		return;
			case 3: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CPhysicalObject>());		return;
			case 4: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CPhysicalObject>());		return;
			case 5: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CPointLight>());			return;
			case 6: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CDirectionalLight>());	return;
			case 7: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CSpotLight>());			return;
			case 8: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CAmbientBoxLight>());	return;
			case 9: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CAmbientSphereLight>());	return;
			case 10: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CMaterialRenderer>());	return;
			case 11: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CAudioSource>());		return;
			case 12: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CAudioListener>());		return;
			case 13: defineButtonsStates(nullptr !=GetTargetActor()->GetComponent<CAnimator>());		return;
			}
		};

		m_componentSelectorWidget = &componentSelectorWidget;
	}

	
	m_inspectorHeader->CreateWidget<UI::Widgets::Visual::Separator>();
	m_destroyedListener = Scene::DestroyedEvent += [this](ActorPtr p_destroyed)
	{ 
		if (p_destroyed == m_targetActor)
			UnFocus();
	};
}

Panels::Inspector::~Inspector()
{
	Scene::DestroyedEvent -= m_destroyedListener;

	UnFocus();
}

void Panels::Inspector::FocusActor(ActorPtr p_target)
{
	if (m_targetActor)
		UnFocus();

	m_actorInfo->RemoveAllWidgets();

	m_targetActor = p_target;
	// TODO tanjp deal component's events
	//m_componentAddedListener = m_targetActor->ComponentAddedEvent += [this] (auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
	//m_componentRemovedListener = m_targetActor->ComponentRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };

	m_inspectorHeader->enabled = true;

	CreateActorInspector(p_target);

    // Force component and script selectors to trigger their ChangedEvent to update button states
	m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);

	EDITOR_EVENT(ActorSelectedEvent).Invoke(m_targetActor);
}

void Panels::Inspector::UnFocus()
{
	if (m_targetActor && m_targetActor->IsAlive())
	{
		// TODO tanjp deal component's events
		//m_targetActor->ComponentAddedEvent		-= m_componentAddedListener;
		//m_targetActor->ComponentRemovedEvent	-= m_componentRemovedListener;
	}

	SoftUnFocus();
}

void Panels::Inspector::SoftUnFocus()
{
    if (m_targetActor)
    {
        EDITOR_EVENT(ActorUnselectedEvent).Invoke(m_targetActor);
        m_inspectorHeader->enabled = false;
        m_targetActor = nullptr;
        m_actorInfo->RemoveAllWidgets();
    }
}

ActorPtr Panels::Inspector::GetTargetActor() const
{
	return m_targetActor;
}

void Panels::Inspector::CreateActorInspector(ActorPtr p_target)
{
	auto components = p_target->GetComponentsInternal();

	// Transform Draw first
	for (auto comp : components){
		if(comp->GetTypeID() == CTransform::GetTypeID()){
			DrawComponent(comp);
		}
	}
	for (auto comp : components){
		if(comp->GetTypeID() != CTransform::GetTypeID()){
			DrawComponent(comp);
		}
	}
}

void Panels::Inspector::DrawComponent(CompPtr p_component)
{
	//if (auto inspectorItem = dynamic_cast<API::IInspectorItem*>(&p_component); inspectorItem)
	{
		auto& header = m_actorInfo->CreateWidget<UI::Widgets::Layout::GroupCollapsable>(p_component->GetName());
		header.closable = p_component->GetTypeID() == LittleEngine:: CTransform::GetTypeID();
		header.CloseEvent += [this, &header, &p_component]
		{ 
			if (p_component->GetActor()->RemoveComponent(p_component))
				m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);
		};
		auto& columns = header.CreateWidget<UI::Widgets::Layout::Columns<2>>();
		columns.widths[0] = 200;
		p_component->OnInspector(columns);
	}
}


void Panels::Inspector::Refresh()
{
	if (m_targetActor)
	{
		m_actorInfo->RemoveAllWidgets();
		CreateActorInspector(m_targetActor);
	}
}

}