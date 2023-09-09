/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "array"

#include "Core/Tools/Utils/PathParser.h"

#include "Modules/UI/Widgets/Texts/TextColored.h"
#include "Modules/UI/Widgets/Drags/DragSingleScalar.h"
#include "Modules/UI/Widgets/Drags/DragMultipleScalars.h"
#include "Modules/UI/Widgets/InputFields/InputText.h"
#include "Modules/UI/Widgets/Selection/ColorEdit.h"
#include "Modules/UI/Widgets/Layout/Group.h"
#include "Modules/UI/Widgets/Layout/Columns.h"
#include "Modules/UI/Widgets/Selection/CheckBox.h"
#include "Modules/UI/Widgets/Buttons/Button.h"
#include "Modules/UI/Widgets/Buttons/ButtonSmall.h"
#include "Modules/UI/Plugins/DDTarget.h"

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/ModelManager.h"
#include "Resource/Manager/TextureManager.h"
#include "Resource/Manager/ShaderManager.h"
#include "Resource/Manager/MaterialManager.h"
#include "Resource/Manager/SoundManager.h"

#include "Modules/Utils/GUIUtil.h"
#include "Resource/Asset/Model.h"

namespace LittleEngine
{

const LittleEngine::Color GUIUtil::TitleColor = { 0.85f, 0.65f, 0.0f };
const LittleEngine::Color GUIUtil::ClearButtonColor = { 0.5f, 0.0f, 0.0f };
const float GUIUtil::_MIN_FLOAT = -999999999.f;
const float GUIUtil::_MAX_FLOAT = +999999999.f;

LittleEngine::UI::Internal::WidgetContainer* GUIUtil::m_root = nullptr;
LittleEngine::Rendering::Resources::Texture* GUIUtil::__EMPTY_TEXTURE = nullptr;

std::vector<std::pair<LittleEngine::UI::Widgets::AWidget*, LittleEngine::UI::Internal::EMemoryMode>>&
	GUIUtil::GetWidgets()
{
	return m_root->GetWidgets();
}

void GUIUtil::ProvideEmptyTexture(LittleEngine::Rendering::Resources::Texture& p_emptyTexture)
{
	__EMPTY_TEXTURE = &p_emptyTexture;
}

void GUIUtil::CreateTitle(const std::string & p_name)
{
	m_root->CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>(p_name, TitleColor);
}

void GUIUtil::DrawBoolean(const std::string & p_name, bool & p_data)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Selection::CheckBox>();
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<bool>>();
	dispatcher.RegisterReference(reinterpret_cast<bool&>(p_data));
}

void GUIUtil::DrawVec2(const std::string & p_name, LittleEngine::FVector2 & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 2>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 2>&>(p_data));
}

void GUIUtil::DrawVec3(const std::string & p_name, LittleEngine::FVector3 & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
}

void GUIUtil::DrawVec4(const std::string & p_name, LittleEngine::FVector4& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
}

void GUIUtil::DrawQuat(const std::string & p_name, LittleEngine::FQuaternion & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
}

void GUIUtil::DrawString(const std::string & p_name, std::string & p_data)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterReference(p_data);
}

void GUIUtil::DrawColor(const std::string & p_name, LittleEngine::Color & p_color, bool p_hasAlpha)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::Color>>();
	dispatcher.RegisterReference(p_color);
}

LittleEngine::UI::Widgets::Texts::Text& GUIUtil::DrawMesh(const std::string & p_name, LittleEngine::Rendering::Resources::Model *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle( p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = m_root->CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (LittleEngine::Utils::PathParser::GetFileType(p_receivedData.first) == LittleEngine::Utils::PathParser::EFileType::MODEL)
		{
			if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::ModelManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

LittleEngine::UI::Widgets::Visual::Image& GUIUtil::DrawTexture(const std::string & p_name, LittleEngine::Rendering::Resources::Texture *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle( p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = m_root->CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Visual::Image>(p_data ? p_data->id : (__EMPTY_TEXTURE ? __EMPTY_TEXTURE->id : 0), LittleEngine::FVector2{ 75, 75 });

	widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (LittleEngine::Utils::PathParser::GetFileType(p_receivedData.first) == LittleEngine::Utils::PathParser::EFileType::TEXTURE)
		{
			if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::TextureManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.textureID.id = resource->id;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::Button>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.textureID.id = (__EMPTY_TEXTURE ? __EMPTY_TEXTURE->id : 0);
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

LittleEngine::UI::Widgets::Texts::Text& GUIUtil::DrawShader(const std::string & p_name, LittleEngine::Rendering::Resources::Shader *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle( p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = m_root->CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (LittleEngine::Utils::PathParser::GetFileType(p_receivedData.first) == LittleEngine::Utils::PathParser::EFileType::SHADER)
		{
			if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::ShaderManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

LittleEngine::UI::Widgets::Texts::Text& GUIUtil::DrawMaterial(const std::string & p_name, LittleEngine::Resources::Material *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle( p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = m_root->CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto& widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (LittleEngine::Utils::PathParser::GetFileType(p_receivedData.first) == LittleEngine::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::MaterialManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

LittleEngine::UI::Widgets::Texts::Text& GUIUtil::DrawSound(const std::string& p_name, LittleEngine::Audio::Resources::Sound*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle( p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = m_root->CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

	auto & widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(displayedText);

	widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
	{
		if (LittleEngine::Utils::PathParser::GetFileType(p_receivedData.first) == LittleEngine::Utils::PathParser::EFileType::SOUND)
		{
			if (auto resource = OVSERVICE(LittleEngine::ResourceManagement::SoundManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
				if (p_updateNotifier)
					p_updateNotifier->Invoke();
			}
		}
	};

	widget.lineBreak = false;

	auto & resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
	{
		p_data = nullptr;
		widget.content = "Empty";
		if (p_updateNotifier)
			p_updateNotifier->Invoke();
	};

	return widget;
}

LittleEngine::UI::Widgets::Texts::Text& GUIUtil::DrawAsset(const std::string& p_name, std::string& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
    CreateTitle( p_name);

    const std::string displayedText = (p_data.empty() ? std::string("Empty") : p_data);
    auto& rightSide = m_root->CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

    auto& widget = rightSide.CreateWidget<LittleEngine::UI::Widgets::Texts::Text>(displayedText);

    widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [&widget, &p_data, p_updateNotifier](auto p_receivedData)
    {
        p_data = p_receivedData.first;
        widget.content = p_receivedData.first;
        if (p_updateNotifier)
            p_updateNotifier->Invoke();
    };

    widget.lineBreak = false;

    auto& resetButton = rightSide.CreateWidget<LittleEngine::UI::Widgets::Buttons::ButtonSmall>("Clear");
    resetButton.idleBackgroundColor = ClearButtonColor;
    resetButton.ClickedEvent += [&widget, &p_data, p_updateNotifier]
    {
        p_data = "";
        widget.content = "Empty";
        if (p_updateNotifier)
            p_updateNotifier->Invoke();
    };

    return widget;
}

void GUIUtil::DrawBoolean(const std::string & p_name, std::function<bool(void)> p_gatherer, std::function<void(bool)> p_provider)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Selection::CheckBox>();
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<bool>>();

	dispatcher.RegisterGatherer([p_gatherer]()
	{
		bool value = p_gatherer();
		return reinterpret_cast<bool&>(value);
	});

	dispatcher.RegisterProvider([p_provider](bool p_value)
	{
		p_provider(reinterpret_cast<bool&>(p_value));
	});
}

void GUIUtil::DrawVec2(const std::string & p_name, std::function<LittleEngine::FVector2(void)> p_gatherer, std::function<void(LittleEngine::FVector2)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 2>>>();
	
	dispatcher.RegisterGatherer([p_gatherer]()
	{
		LittleEngine::FVector2 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 2>&>(value);
	});

	dispatcher.RegisterProvider([p_provider](std::array<float, 2> p_value)
	{
		p_provider(reinterpret_cast<LittleEngine::FVector2&>(p_value));
	});
}

void GUIUtil::DrawVec3(const std::string & p_name, std::function<LittleEngine::FVector3(void)> p_gatherer, std::function<void(LittleEngine::FVector3)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();

	dispatcher.RegisterGatherer([p_gatherer]()
	{
		LittleEngine::FVector3 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 3>&>(value);
	});

	dispatcher.RegisterProvider([p_provider](std::array<float, 3> p_value)
	{
		p_provider(reinterpret_cast<LittleEngine::FVector3&>(p_value));
	});
}

void GUIUtil::DrawVec4(const std::string & p_name, std::function<LittleEngine::FVector4(void)> p_gatherer, std::function<void(LittleEngine::FVector4)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	
	dispatcher.RegisterGatherer([p_gatherer]()
	{
		LittleEngine::FVector4 value = p_gatherer();
		return reinterpret_cast<const std::array<float, 4>&>(value);
	});

	dispatcher.RegisterProvider([p_provider](std::array<float, 4> p_value)
	{
		p_provider(reinterpret_cast<LittleEngine::FVector4&>(p_value));
	});
}

void GUIUtil::DrawQuat(const std::string & p_name, std::function<LittleEngine::FQuaternion(void)> p_gatherer, std::function<void(LittleEngine::FQuaternion)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	
	dispatcher.RegisterGatherer([p_gatherer]()
	{
		LittleEngine::FQuaternion value = p_gatherer();
		return reinterpret_cast<const std::array<float, 4>&>(value);
	});

	dispatcher.RegisterProvider([&dispatcher, p_provider](std::array<float, 4> p_value)
	{
		p_provider(LittleEngine::FQuaternion::Normalize(reinterpret_cast<LittleEngine::FQuaternion&>(p_value)));
	});
}

void GUIUtil::DrawDDString(const std::string& p_name,
	std::function<std::string()> p_gatherer, std::function<void(std::string)> p_provider,
	const std::string& p_identifier)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);

	auto& ddTarget = widget.AddPlugin<LittleEngine::UI::Plugins::DDTarget<std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*>>>(p_identifier);
	ddTarget.DataReceivedEvent += [&widget, &dispatcher](std::pair<std::string, LittleEngine::UI::Widgets::Layout::Group*> p_data)
	{
		widget.content = p_data.first;
		dispatcher.NotifyChange();
	};
}

void GUIUtil::DrawString(const std::string & p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}

void GUIUtil::DrawColor(const std::string & p_name, std::function<LittleEngine::Color(void)> p_gatherer, std::function<void(LittleEngine::Color)> p_provider, bool p_hasAlpha)
{
	CreateTitle( p_name);
	auto& widget = m_root->CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::Color>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}
	
}
