/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



#include "Modules/Framework/SceneSystem/Scene.h"

namespace LittleEngine
{

	
	/**
	* The scene manager of the current scene
	*/
	class SceneManager
	{
	public:

		/** 
		* Default constructor
		* @param p_sceneRootFolder (Optional)
		*/
		SceneManager(const std::string& p_sceneRootFolder = "");

		/**
		* Default destructor
		*/
		~SceneManager();

		/**
		* Update
		*/
		void Update();

		/**
		* Load an play a scene with a delay
		* @param p_path
		* @param p_absolute
		*/
		void LoadAndPlayDelayed(const std::string& p_path, bool p_absolute = false);

		/**
		* Load an empty scene in memory
		*/
		void LoadEmptyScene();

		/**
		* Load an empty lighted scene in memory
		*/
		void LoadEmptyLightedScene();

		/**
		* Load specific scene in memory
		* @param p_scenePath
		* @param p_absolute (If this setting is set to true, the scene loader will ignore the "SceneRootFolder" given on SceneManager construction)
		*/
		bool LoadScene(const std::string& p_path, bool p_absolute = false);

		bool SaveScene(const std::string& p_path);
		std::string BackupScene();
		/**
		* Load specific scene in memory
		* @param p_scenePath
		*/
		bool LoadSceneFromMemory(const std::string& p_sceneStr);

		/**
		* Destroy current scene from memory
		*/
		void UnloadCurrentScene();

		/**
		* Return true if a scene is currently loaded
		*/
		bool HasCurrentScene() const;

		/*
		* Return current loaded scene
		*/
		Scene* GetCurrentScene();

		/**
		* Return the current scene source path
		*/
		std::string GetCurrentSceneSourcePath() const;

		/**
		* Return true if the currently loaded scene has been loaded from a file
		*/
		bool IsCurrentSceneLoadedFromDisk() const;

		/**
		* Store the given path as the current scene source path
		* @param p_path
		*/
		void StoreCurrentSceneSourcePath(const std::string& p_path);

		/**
		* Reset the current scene source path to an empty string
		*/
		void ForgetCurrentSceneSourcePath();

	public:
		LittleEngine::Eventing::Event<Scene*> SceneLoadEvent;
		LittleEngine::Eventing::Event<> SceneUnloadEvent;
		LittleEngine::Eventing::Event<const std::string&> CurrentSceneSourcePathChangedEvent;

	private:
		const std::string m_sceneRootFolder;
		Scene* m_currentScene = nullptr;

		bool m_currentSceneLoadedFromPath = false;
		std::string m_currentSceneSourcePath = "";

		std::function<void()> m_delayedLoadCall;
	};
}
