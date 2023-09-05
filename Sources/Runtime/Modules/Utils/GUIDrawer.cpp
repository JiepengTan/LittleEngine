/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <array>

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
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/SoundManager.h"

#include "Modules/Utils/GUIDrawer.h"
#include "Modules/Rendering/Resources/Model.h"

const LittleEngine::UI::Types::Color LittleEngine::Helpers::GUIDrawer::TitleColor = { 0.85f, 0.65f, 0.0f };
const LittleEngine::UI::Types::Color LittleEngine::Helpers::GUIDrawer::ClearButtonColor = { 0.5f, 0.0f, 0.0f };
const float LittleEngine::Helpers::GUIDrawer::_MIN_FLOAT = -999999999.f;
const float LittleEngine::Helpers::GUIDrawer::_MAX_FLOAT = +999999999.f;
LittleEngine::Rendering::Resources::Texture* LittleEngine::Helpers::GUIDrawer::__EMPTY_TEXTURE = nullptr;

void LittleEngine::Helpers::GUIDrawer::ProvideEmptyTexture(LittleEngine::Rendering::Resources::Texture& p_emptyTexture)
{
	__EMPTY_TEXTURE = &p_emptyTexture;
}

void LittleEngine::Helpers::GUIDrawer::CreateTitle(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string & p_name)
{
	p_root.CreateWidget<LittleEngine::UI::Widgets::Texts::TextColored>(p_name, TitleColor);
}

void LittleEngine::Helpers::GUIDrawer::DrawBoolean(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, bool & p_data)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::CheckBox>();
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<bool>>();
	dispatcher.RegisterReference(reinterpret_cast<bool&>(p_data));
}

void LittleEngine::Helpers::GUIDrawer::DrawVec2(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::FVector2 & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 2>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 2>&>(p_data));
}

void LittleEngine::Helpers::GUIDrawer::DrawVec3(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::FVector3 & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 3>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 3>&>(p_data));
}

void LittleEngine::Helpers::GUIDrawer::DrawVec4(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::FVector4& p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
}

void LittleEngine::Helpers::GUIDrawer::DrawQuat(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::FQuaternion & p_data, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::array<float, 4>>>();
	dispatcher.RegisterReference(reinterpret_cast<std::array<float, 4>&>(p_data));
}

void LittleEngine::Helpers::GUIDrawer::DrawString(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::string & p_data)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterReference(p_data);
}

void LittleEngine::Helpers::GUIDrawer::DrawColor(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::UI::Types::Color & p_color, bool p_hasAlpha)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::UI::Types::Color>>();
	dispatcher.RegisterReference(p_color);
}

LittleEngine::UI::Widgets::Texts::Text& LittleEngine::Helpers::GUIDrawer::DrawMesh(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::Rendering::Resources::Model *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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

LittleEngine::UI::Widgets::Visual::Image& LittleEngine::Helpers::GUIDrawer::DrawTexture(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::Rendering::Resources::Texture *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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

LittleEngine::UI::Widgets::Texts::Text& LittleEngine::Helpers::GUIDrawer::DrawShader(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::Rendering::Resources::Shader *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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

LittleEngine::UI::Widgets::Texts::Text& LittleEngine::Helpers::GUIDrawer::DrawMaterial(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, LittleEngine::Resources::Material *& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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

LittleEngine::UI::Widgets::Texts::Text& LittleEngine::Helpers::GUIDrawer::DrawSound(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, LittleEngine::Audio::Resources::Sound*& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
	CreateTitle(p_root, p_name);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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

LittleEngine::UI::Widgets::Texts::Text& LittleEngine::Helpers::GUIDrawer::DrawAsset(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name, std::string& p_data, LittleEngine::Eventing::Event<>* p_updateNotifier)
{
    CreateTitle(p_root, p_name);

    const std::string displayedText = (p_data.empty() ? std::string("Empty") : p_data);
    auto& rightSide = p_root.CreateWidget<LittleEngine::UI::Widgets::Layout::Group>();

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

void LittleEngine::Helpers::GUIDrawer::DrawBoolean(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<bool(void)> p_gatherer, std::function<void(bool)> p_provider)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::CheckBox>();
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

void LittleEngine::Helpers::GUIDrawer::DrawVec2(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<LittleEngine::FVector2(void)> p_gatherer, std::function<void(LittleEngine::FVector2)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 2>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
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

void LittleEngine::Helpers::GUIDrawer::DrawVec3(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<LittleEngine::FVector3(void)> p_gatherer, std::function<void(LittleEngine::FVector3)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 3>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
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

void LittleEngine::Helpers::GUIDrawer::DrawVec4(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<LittleEngine::FVector4(void)> p_gatherer, std::function<void(LittleEngine::FVector4)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
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

void LittleEngine::Helpers::GUIDrawer::DrawQuat(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<LittleEngine::FQuaternion(void)> p_gatherer, std::function<void(LittleEngine::FQuaternion)> p_provider, float p_step, float p_min, float p_max)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Drags::DragMultipleScalars<float, 4>>(GetDataType<float>(), p_min, p_max, 0.f, p_step, "", GetFormat<float>());
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

void LittleEngine::Helpers::GUIDrawer::DrawDDString(LittleEngine::UI::Internal::WidgetContainer& p_root, const std::string& p_name,
	std::function<std::string()> p_gatherer, std::function<void(std::string)> p_provider,
	const std::string& p_identifier)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>("");
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

void LittleEngine::Helpers::GUIDrawer::DrawString(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<std::string(void)> p_gatherer, std::function<void(std::string)> p_provider)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::InputFields::InputText>("");
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<std::string>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}

void LittleEngine::Helpers::GUIDrawer::DrawColor(LittleEngine::UI::Internal::WidgetContainer & p_root, const std::string & p_name, std::function<LittleEngine::UI::Types::Color(void)> p_gatherer, std::function<void(LittleEngine::UI::Types::Color)> p_provider, bool p_hasAlpha)
{
	CreateTitle(p_root, p_name);
	auto& widget = p_root.CreateWidget<LittleEngine::UI::Widgets::Selection::ColorEdit>(p_hasAlpha);
	auto& dispatcher = widget.AddPlugin<LittleEngine::UI::Plugins::DataDispatcher<LittleEngine::UI::Types::Color>>();
	dispatcher.RegisterGatherer(p_gatherer);
	dispatcher.RegisterProvider(p_provider);
}
