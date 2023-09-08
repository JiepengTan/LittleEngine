/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "tinyxml2/tinyxml2.h"
#include "Platform/Windowing/Dialogs/MessageBox.h"

#include "Modules/Framework/SceneSystem/SceneManager.h"

#include "Core/Tools/Filesystem/FileUtil.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/TextAssetManager.h"
#include "Resource/ResIncludeScene.h"

LittleEngine::SceneManager::SceneManager(const std::string& p_sceneRootFolder) : m_sceneRootFolder(p_sceneRootFolder)
{
	LoadEmptyScene();
}

LittleEngine::SceneManager::~SceneManager()
{
	UnloadCurrentScene();
}

void LittleEngine::SceneManager::Update()
{
	if (m_delayedLoadCall)
	{
		m_delayedLoadCall();
		m_delayedLoadCall = 0;
	}
}

void LittleEngine::SceneManager::LoadAndPlayDelayed(const std::string& p_path, bool p_absolute)
{
	m_delayedLoadCall = [this, p_path, p_absolute]
	{
		std::string previousSourcePath = GetCurrentSceneSourcePath();
		LoadScene(p_path, p_absolute);
		StoreCurrentSceneSourcePath(previousSourcePath);
		GetCurrentScene()->Play();
	};
}

void LittleEngine::SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();
}

void LittleEngine::SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();

	auto directionalLight = m_currentScene->CreateActor("Directional Light");
	directionalLight->AddComponent<CDirectionalLight>()->SetIntensity(0.75f);
	directionalLight->transform->SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	directionalLight->transform->SetLocalRotation(LittleEngine::FQuaternion({ 120.0f, -40.0f, 0.0f }));

	auto ambientLight = m_currentScene->CreateActor("Ambient Light");
	ambientLight->AddComponent<CAmbientSphereLight>()->SetRadius(10000.0f);

	auto camera = m_currentScene->CreateActor("Main Camera");
	camera->AddComponent<CCamera>();
	camera->transform->SetLocalPosition({ 0.0f, 3.0f, 8.0f });
	camera->transform->SetLocalRotation(LittleEngine::FQuaternion({ 20.0f, 180.0f, 0.0f }));
}

bool LittleEngine::SceneManager::LoadScene(const std::string& p_path, bool p_absolute)
{
	std::string completePath = (p_absolute ? "" : m_sceneRootFolder) + p_path;
	auto textAsset = GetGlobalService<LittleEngine::ResourceManagement::TextAssetManager>().LoadResource(completePath);
	if (LoadSceneFromMemory(textAsset->text))
	{
		StoreCurrentSceneSourcePath(completePath);
		return true;
	}

	return false;
}

bool LittleEngine::SceneManager::SaveScene(const std::string& p_path)
{
	StoreCurrentSceneSourcePath(p_path);
	auto fullPath = FileUtil::GetRealPath(p_path);
	std::string context;
	// TODO tanjp
	//tinyxml2::XMLDocument doc;
	//tinyxml2::XMLNode* node = doc.NewElement("root");
	//doc.InsertFirstChild(node);
	//m_context.sceneManager.GetCurrentScene()->OnSerialize(doc, node);
	//doc.SaveFile(p_path.c_str());
	FileUtil::WriteAllText(fullPath,context);
}

bool LittleEngine::SceneManager::LoadSceneFromMemory(const std::string& p_sceneStr)
{
	LoadEmptyScene();
	std::string error;
	auto&& json = Json::parse(p_sceneStr, error);
	ResScene resScene;
	JsonSerializer::Read(json, resScene);
	// TODO recover scene
	//m_currentScene->OnDeserialize(p_serializer);
	LittleEngine::Windowing::Dialogs::MessageBox message("Scene loading failed", "The scene you are trying to load was not found or corrupted", LittleEngine::Windowing::Dialogs::MessageBox::EMessageType::ERROR, LittleEngine::Windowing::Dialogs::MessageBox::EButtonLayout::OK, true);
	return false;
}

void LittleEngine::SceneManager::UnloadCurrentScene()
{
	if (m_currentScene)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		SceneUnloadEvent.Invoke();
	}

	ForgetCurrentSceneSourcePath();
}

bool LittleEngine::SceneManager::HasCurrentScene() const
{
	return m_currentScene;
}

LittleEngine::Scene* LittleEngine::SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

std::string LittleEngine::SceneManager::GetCurrentSceneSourcePath() const
{
	return m_currentSceneSourcePath;
}

bool LittleEngine::SceneManager::IsCurrentSceneLoadedFromDisk() const
{
	return m_currentSceneLoadedFromPath;
}

void LittleEngine::SceneManager::StoreCurrentSceneSourcePath(const std::string& p_path)
{
	m_currentSceneSourcePath = p_path;
	m_currentSceneLoadedFromPath = true;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}

void LittleEngine::SceneManager::ForgetCurrentSceneSourcePath()
{
	m_currentSceneSourcePath = "";
	m_currentSceneLoadedFromPath = false;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}
