/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "map"

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
#include "Resource/Manager/ModelManager.h"
#include "Resource/Manager/TextureManager.h"
#include "Resource/Manager/ShaderManager.h"

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
#include "Core/Reflection/MetaDefine.h"
#include "Core/Reflection/TypeUtil.h"
#include "Modules/UI/Widgets/Layout/InspectorProxy.h"

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
	GUIUtil::m_root =&headerColumns;
	/* Name field */
	auto nameGatherer = [this] { return m_targetActor ? m_targetActor->GetName() : "%undef%"; };
	auto nameProvider = [this](const std::string& p_newName) { if (m_targetActor) m_targetActor->SetName(p_newName); };
	GUIUtil::DrawString( "Name", nameGatherer, nameProvider);

	/* Tag field */
	auto tagGatherer = [this] { return m_targetActor ? m_targetActor->GetTag() : "%undef%"; };
	auto tagProvider = [this](const std::string & p_newName) { if (m_targetActor) m_targetActor->SetTag(p_newName); };
	GUIUtil::DrawString( "Tag", tagGatherer, tagProvider);

	/* Active field */
	auto activeGatherer = [this] { return m_targetActor ? m_targetActor->IsSelfActive() : false; };
	auto activeProvider = [this](bool p_active) { if (m_targetActor) m_targetActor->SetActive(p_active); };
	GUIUtil::DrawBoolean( "Active", activeGatherer, activeProvider);

	/* Component select + button */
	{
		m_componentSelectorWidget = &m_inspectorHeader->CreateWidget<UI::Widgets::Selection::ComboBox>(0);
		m_addComponentButton = &m_inspectorHeader->CreateWidget<UI::Widgets::Buttons::Button>("Add Component", FVector2{ 100.f, 0 });
	
		RefreshAddComponentPanel();
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
		if(comp->GetTypeID() == CTransform::GetStaticTypeID()){
			DrawComponent(comp);
		}
	}
	for (auto comp : components){
		if(comp->GetTypeID() != CTransform::GetStaticTypeID()){
			DrawComponent(comp);
		}
	}
		
	RefreshAddComponentPanel();
}

void Panels::Inspector::RefreshAddComponentPanel()
{
		
	m_componentSelectorWidget->choices.clear();
	m_componentSelectorWidget->ValueChangedEvent.RemoveAllListeners();
	m_addComponentButton->ClickedEvent.RemoveAllListeners();
	m_componentSelectorWidget->lineBreak = false;
	auto types = TypeUtil::GetAllTypes();
	TVector<TypeInfoPtr> compTypes ;
	int idx = 0;
	for (auto type : types)
	{
		if(!type->IsSubclassOf(Component::GetStaticTypeID())
			|| type->IsAbstract()
			|| type->IsAssignableFrom<CTransform>()
		)
			continue;
		if(GetTargetActor() != nullptr &&GetTargetActor()->HasComponent(type->GetTypeID()))
			continue;
		m_componentSelectorWidget->choices.emplace(idx++, type->GetTypeName());
		compTypes.push_back(type);
	}
		
	m_addComponentButton->idleBackgroundColor = Color{ 0.7f, 0.5f, 0.f };
	m_addComponentButton->textColor = Color::White;
	m_addComponentButton->ClickedEvent += [this]
	{
		auto typeName = m_componentSelectorWidget->choices.at(m_componentSelectorWidget->currentChoice);
		auto clickType =TypeUtil::GetType(typeName);
		LOG_INFO("Click " + typeName + " typeId " +std::to_string( clickType->GetTypeID()));
		GetTargetActor()->AddComponent(clickType->GetTypeID());
		m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);
		EDITOR_EXEC(DelayAction([this] { Refresh(); }));
	};

	m_componentSelectorWidget->ValueChangedEvent += [this,compTypes](int p_value)
	{
		auto defineButtonsStates = [this](bool p_componentExists)
		{
			m_addComponentButton->disabled = p_componentExists;
			m_addComponentButton->idleBackgroundColor = !p_componentExists ? Color{ 0.7f, 0.5f, 0.f } : Color{ 0.1f, 0.1f, 0.1f };
		};
		auto type= compTypes.at(p_value);
		auto comp =GetTargetActor()->GetComponent(type->GetTypeID());
		defineButtonsStates(nullptr !=comp);	
	};
}
void Panels::Inspector::DrawComponent(CompPtr p_component)
{
	//if (auto inspectorItem = dynamic_cast<API::IInspectorItem*>(&p_component); inspectorItem)
	{
		auto& header = m_actorInfo->CreateWidget<UI::Widgets::Layout::GroupCollapsable>(p_component->GetTypeName());
		header.closable = p_component->GetTypeID() != LittleEngine:: CTransform::GetStaticTypeID();
		header.CloseEvent += [this, &header, p_component]
		{
			auto actor= p_component->GetActor();
			if (actor->RemoveComponent(p_component))
			{
				m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);
				EDITOR_EXEC(DelayAction([this] { Refresh(); }));
			}
		};
		auto& columns = header.CreateWidget<UI::Widgets::Layout::Columns<2>>();
		columns.widths[0] = GUIUtil::_DEFAULT_COLUME_WIDTH;
		GUIUtil::m_root = &columns;
		p_component->OnInspector();
		auto type = p_component->GetType();
		if(type->HasMeta(MetaDefine::CustomerEditor))
		{
			auto& inspectorProxy = header.CreateWidget<UI::Widgets::Layout::InspectorProxy>();
			inspectorProxy.SetDrawCallback([p_component]()
			{
				p_component->OnInspectorGUI();
			});
		}
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
