/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "filesystem"

#include "Modules/Rendering/Entities/Light.h"
#include "Modules/Framework/Global/ServiceLocator.h"

#include "../Editor/Core/Context.h"

#include "Core/Tools/Filesystem/FileUtil.h"
#include "Core/Tools/Filesystem/PathUtil.h"
#include "Resource/Manager/AnimationManager.h"

using namespace LittleEngine::Global;
using namespace LittleEngine::ResourceManagement;

LittleEngine::Editor::Core::Context::Context(const std::string& p_projectPath, const std::string& p_projectName) :
	projectPath(p_projectPath),
	projectName(p_projectName),
	projectFilePath(p_projectPath + p_projectName + ".project"),
	engineAssetsPath(std::filesystem::canonical("Data\\Engine").string() + "\\"),
	projectAssetsPath(p_projectPath + "Assets\\"),
	projectScriptsPath(p_projectPath + "Scripts\\"),
	editorAssetsPath("Data\\Editor\\"),
	sceneManager(projectAssetsPath),
	projectSettings(projectFilePath),
	editSettings(p_projectPath + "edit.ini")
{
	if (!IsProjectSettingsIntegrityVerified())
	{
		ResetProjectSettings();
		projectSettings.Rewrite();
	}
	PathUtil::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	ModelManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	TextureManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	ShaderManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	MaterialManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	SoundManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	AnimationManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	TextAssetManager::ProvideAssetPaths(projectAssetsPath, engineAssetsPath);
	
	// init managers
	ServiceLocator::Provide<ModelManager>(modelManager);
	ServiceLocator::Provide<TextureManager>(textureManager);
	ServiceLocator::Provide<ShaderManager>(shaderManager);
	ServiceLocator::Provide<MaterialManager>(materialManager);
	ServiceLocator::Provide<SoundManager>(soundManager);
	ServiceLocator::Provide<AnimationManager>(animationManager);
	ServiceLocator::Provide<TextAssetManager>(textAssetManager);

	
	LOG_INFO("DebugProgress EditorContext::Init ");
	
	/* Settings */
	LittleEngine::Windowing::Settings::DeviceSettings deviceSettings;
	deviceSettings.contextMajorVersion = 4;
	deviceSettings.contextMinorVersion = 3;
	windowSettings.title = "Little Editor";
	windowSettings.width = 1280;
	windowSettings.height = 720;
	windowSettings.maximized = true;
	;
	auto logoData = Resources::TextureLoader::LoadRawData(editorAssetsPath +  "Textures/Icons/Logo_Engine.png");
	/* Window creation */
	device = std::make_unique<LittleEngine::Windowing::Context::Device>(deviceSettings);
	window = std::make_unique<LittleEngine::Windowing::Window>(*device, windowSettings);
	window->SetIconFromMemory(reinterpret_cast<uint8_t*>(logoData), 16, 16);
	inputManager = std::make_unique<LittleEngine::Windowing::Inputs::InputManager>(*window);
	window->MakeCurrentContext();

	device->SetVsync(true);
	
	
	LOG_INFO("DebugProgress EditorContext::create devices ");
	/* Graphics context creation */
	driver = std::make_unique<LittleEngine::Rendering::Context::Driver>(LittleEngine::Rendering::Settings::DriverSettings{ true });
	LOG_INFO("DebugProgress EditorContext::renderer Init");
	renderer = std::make_unique<LittleEngine::SceneRenderer>(*driver);
	renderer->SetCapability(LittleEngine::Rendering::Settings::ERenderingCapability::MULTISAMPLE, true);
	LOG_INFO("DebugProgress EditorContext::shapeDrawer Init");
	shapeDrawer = std::make_unique<LittleEngine::Rendering::Core::ShapeDrawer>(*renderer);

	LOG_INFO("DebugProgress EditorContext::create_directories Init  ");
	std::filesystem::create_directories(std::string(getenv("APPDATA")) + "\\OverloadTech\\LittleEditor\\");
	
	LOG_INFO("DebugProgress EditorContext::uiManager Init  ");
	uiManager = std::make_unique<LittleEngine::UI::Core::UIManager>(window->GetGlfwWindow(), LittleEngine::UI::Styling::EStyle::ALTERNATIVE_DARK);
	uiManager->LoadFont("Ruda_Big", editorAssetsPath + "\\Fonts\\Ruda-Bold.ttf", 16);
	uiManager->LoadFont("Ruda_Small", editorAssetsPath + "\\Fonts\\Ruda-Bold.ttf", 12);
	uiManager->LoadFont("Ruda_Medium", editorAssetsPath + "\\Fonts\\Ruda-Bold.ttf", 14);
	uiManager->UseFont("Ruda_Medium");
	uiManager->SetEditorLayoutSaveFilename(std::string(getenv("APPDATA")) + "\\OverloadTech\\LittleEditor\\layout.ini");
	uiManager->SetEditorLayoutAutosaveFrequency(60.0f);
	uiManager->EnableEditorLayoutSave(true);
	uiManager->EnableDocking(true);

	if (!std::filesystem::exists(std::string(getenv("APPDATA")) + "\\OverloadTech\\LittleEditor\\layout.ini"))
		uiManager->ResetLayout("Config\\layout.ini");

	LOG_INFO("DebugProgress EditorContext::audioEngine Init  ");
	/* Audio */
	audioEngine = std::make_unique<LittleEngine::Audio::Core::AudioEngine>(projectAssetsPath);
	audioPlayer = std::make_unique<LittleEngine::Audio::Core::AudioPlayer>(*audioEngine);

	LOG_INFO("DebugProgress EditorContext::editorResources Init  ");
	/* Editor resources */
	editorResources = std::make_unique<LittleEngine::Editor::Core::EditorResources>(editorAssetsPath);

	LOG_INFO("DebugProgress EditorContext::physicsEngine Init  ");
	/* Physics engine */
	physicsEngine = std::make_unique<LittleEngine::Physics::Core::PhysicsEngine>(LittleEngine::Physics::Settings::PhysicsSettings{ {0.0f, -9.81f, 0.0f } });

	LOG_INFO("DebugProgress EditorContext::ServiceLocator Init  ");
	/* Service Locator providing */
	ServiceLocator::Provide<LittleEngine::Physics::Core::PhysicsEngine>(*physicsEngine);
	ServiceLocator::Provide<LittleEngine::Windowing::Inputs::InputManager>(*inputManager);
	ServiceLocator::Provide<LittleEngine::Windowing::Window>(*window);
	ServiceLocator::Provide<LittleEngine::SceneManager>(sceneManager);
	ServiceLocator::Provide<LittleEngine::Audio::Core::AudioEngine>(*audioEngine);
	ServiceLocator::Provide<LittleEngine::Audio::Core::AudioPlayer>(*audioPlayer);
	
	LOG_INFO("DebugProgress EditorContext::scriptInterpreter Init  ");
	/* Scripting */
	scriptInterpreter = std::make_unique<LittleEngine::Scripting::ScriptInterpreter>(projectScriptsPath);

	LOG_INFO("DebugProgress EditorContext::engineUBO Init  ");
	engineUBO = std::make_unique<LittleEngine::Rendering::Buffers::UniformBuffer>
	(
		/* UBO Data Layout */
		sizeof(LittleEngine::FMatrix4) +
		sizeof(LittleEngine::FMatrix4) +
		sizeof(LittleEngine::FMatrix4) +
		sizeof(LittleEngine::FVector3) +
		sizeof(float) +
		sizeof(LittleEngine::FMatrix4),
		0, 0,
		LittleEngine::Rendering::Buffers::EAccessSpecifier::STREAM_DRAW
	);

	lightSSBO			= std::make_unique<LittleEngine::Rendering::Buffers::ShaderStorageBuffer>(LittleEngine::Rendering::Buffers::EAccessSpecifier::STREAM_DRAW);
	simulatedLightSSBO	= std::make_unique<LittleEngine::Rendering::Buffers::ShaderStorageBuffer>(LittleEngine::Rendering::Buffers::EAccessSpecifier::STREAM_DRAW); // Used in Asset View

	std::vector<LittleEngine::FMatrix4> simulatedLights;

	LittleEngine::FTransform simulatedLightTransform;
	simulatedLightTransform.SetLocalRotation(LittleEngine::FQuaternion({ 45.f, 180.f, 10.f }));

	LittleEngine::Rendering::Entities::Light simulatedDirectionalLight(simulatedLightTransform, LittleEngine::Rendering::Entities::Light::Type::DIRECTIONAL);
	simulatedDirectionalLight.color = { 1.f, 1.f, 1.f };
	simulatedDirectionalLight.intensity = 1.f;

	LittleEngine::Rendering::Entities::Light simulatedAmbientLight(simulatedLightTransform, LittleEngine::Rendering::Entities::Light::Type::AMBIENT_SPHERE);
	simulatedAmbientLight.color = { 0.07f, 0.07f, 0.07f };
	simulatedAmbientLight.intensity = 1.f;
	simulatedAmbientLight.constant = 1000.0f;

	simulatedLights.push_back(simulatedDirectionalLight.GenerateMatrix());
	simulatedLights.push_back(simulatedAmbientLight.GenerateMatrix());

	simulatedLightSSBO->SendBlocks<LittleEngine::FMatrix4>(simulatedLights.data(), simulatedLights.size() * sizeof(LittleEngine::FMatrix4));

	LOG_INFO("DebugProgress EditorContext::ApplyProjectSettings  ");
	ApplyProjectSettings();
}

LittleEngine::Editor::Core::Context::~Context()
{
	modelManager.UnloadResources();
	textureManager.UnloadResources();
	shaderManager.UnloadResources();
	materialManager.UnloadResources();
	soundManager.UnloadResources();
}

void LittleEngine::Editor::Core::Context::ResetProjectSettings()
{
	projectSettings.RemoveAll();
	projectSettings.Add<float>("gravity", -9.81f);
	projectSettings.Add<int>("x_resolution", 1280);
	projectSettings.Add<int>("y_resolution", 720);
	projectSettings.Add<bool>("fullscreen", false);
	projectSettings.Add<std::string>("executable_name", "Game");
	projectSettings.Add<std::string>("start_scene", "Scene.scene");
	projectSettings.Add<bool>("vsync", true);
	projectSettings.Add<bool>("multisampling", true);
	projectSettings.Add<int>("samples", 4);
	projectSettings.Add<int>("opengl_major", 4);
	projectSettings.Add<int>("opengl_minor", 3);
	projectSettings.Add<bool>("dev_build", true);
}

bool LittleEngine::Editor::Core::Context::IsProjectSettingsIntegrityVerified()
{
	return
		projectSettings.IsKeyExisting("gravity") &&
		projectSettings.IsKeyExisting("x_resolution") &&
		projectSettings.IsKeyExisting("y_resolution") &&
		projectSettings.IsKeyExisting("fullscreen") &&
		projectSettings.IsKeyExisting("executable_name") &&
		projectSettings.IsKeyExisting("start_scene") &&
		projectSettings.IsKeyExisting("vsync") &&
		projectSettings.IsKeyExisting("multisampling") &&
		projectSettings.IsKeyExisting("samples") &&
		projectSettings.IsKeyExisting("opengl_major") &&
		projectSettings.IsKeyExisting("opengl_minor") &&
		projectSettings.IsKeyExisting("dev_build");
}

void LittleEngine::Editor::Core::Context::ApplyProjectSettings()
{
	physicsEngine->SetGravity({ 0.0f, projectSettings.Get<float>("gravity"), 0.0f });
}
