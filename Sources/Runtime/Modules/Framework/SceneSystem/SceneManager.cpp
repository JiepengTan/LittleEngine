/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "tinyxml2/tinyxml2.h"
#include "Platform/Windowing/Dialogs/MessageBox.h"

#include "Modules/Framework/SceneSystem/SceneManager.h"

#include "Core/Tools/Filesystem/FileUtil.h"
#include "Core/Tools/Filesystem/PathUtil.h"
#include "Modules/Framework/ECS/Components/CDirectionalLight.h"
#include "Modules/Framework/ECS/Components/CAmbientSphereLight.h"
#include "Modules/Framework/ECS/Components/CCamera.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/TextAssetManager.h"
#include "Resource/Data/ResIncludeScene.h"
#include "_Generated/Serializer/AllSerializer.h"

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

	SceneLoadEvent.Invoke(m_currentScene);
}

void LittleEngine::SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke(m_currentScene);

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
	LOG_INFO("LoadScene =" + p_path);
	auto textAsset = GetGlobalService<LittleEngine::ResourceManagement::TextAssetManager>().LoadResource(p_path);
        try { 
	        if (LoadSceneFromMemory(textAsset->text))
	        {
        		LOG_INFO("LoadScene succ =" + p_path);
        		std::string completePath = PathUtil::GetRealPath(p_path);
        		StoreCurrentSceneSourcePath(completePath);
        		return true;
	        }
        } catch (std::exception ex) {
			LOG_ERROR("LoadScene faild =" + p_path);
        }
	LOG_ERROR("LoadScene faild =" + p_path);
	return false;
}
std::string LittleEngine::SceneManager::BackupScene()
{
	ResScene resScene;
	m_currentScene->SaveTo(resScene);
	auto&& json = JsonSerializer::Write(resScene);
	return json.dump(); 
}
bool LittleEngine::SceneManager::SaveScene(const std::string& p_path)
{
	auto fullPath = PathUtil::GetRealPath(p_path);
	ResScene resScene;
	m_currentScene->SaveTo(resScene);
	auto&& json = JsonSerializer::Write(resScene);
	std::string&& context = json.dump(); 
	LOG_INFO(""+context);
	FileUtil::WriteAllText(fullPath,context);
	StoreCurrentSceneSourcePath(p_path);
	return true;
}

bool LittleEngine::SceneManager::LoadSceneFromMemory(const std::string& p_sceneStr)
{
	std::string error;
	auto&& asset_json = Json::parse(p_sceneStr, error);
	if(!error.empty())
	{
		LOG_ERROR( + "Load Scene error : " + error);
		return false;
	}
	ResScene resScene;
	JsonSerializer::Read(asset_json,resScene);
	LoadEmptyScene();
	m_currentScene->LoadFrom(resScene);
	SceneLoadEvent.Invoke(m_currentScene);
	return true;
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
