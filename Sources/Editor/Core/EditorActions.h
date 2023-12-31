/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Core/Tools/Filesystem/IniFile.h"
#include "Core/CoreInclude.h"
#include "Modules/Framework/ECS/Actor.h"
#include "Core/Tools/Utils/PathParser.h"

#include "../Editor/Core/Context.h"
#include "../Editor/Core/EditorRenderer.h"
#include "../Editor/Core/PanelsManager.h"

#define EDITOR_EXEC(action)					GetGlobalService<LittleEngine::Editor::Core::EditorActions>().action
#define EDITOR_BIND(method, ...)			std::bind(&LittleEngine::Editor::Core::EditorActions::method, &GetGlobalService<LittleEngine::Editor::Core::EditorActions>(), ##__VA_ARGS__)
#define EDITOR_EVENT(target)				GetGlobalService<LittleEngine::Editor::Core::EditorActions>().target
#define EDITOR_CONTEXT(instance)			GetGlobalService<LittleEngine::Editor::Core::EditorActions>().GetContext().instance
#define EDITOR_RENDERER()					GetGlobalService<LittleEngine::Editor::Core::EditorActions>().GetRenderer()
#define EDITOR_PANEL(type, id)				GetGlobalService<LittleEngine::Editor::Core::EditorActions>().GetPanelsManager().GetPanelAs<type>(id)

namespace LittleEngine::Editor::Core
{
	/**
	* A set of editor actions
	*/
	class EditorActions
	{
	public:
		/**
		* Constructor
		* @param p_context
		* @param p_editorRenderer
		* @param p_panelsManager
		*/
		EditorActions(Context& p_context, EditorRenderer& p_editorRenderer, PanelsManager& p_panelsManager);

		#pragma region TOOLS
		/**
		* Returns the context
		*/
		Context& GetContext();

		/**
		* Returns the renderer
		*/
		EditorRenderer& GetRenderer();

		/**
		* Returns the panels manager
		*/
		PanelsManager& GetPanelsManager();
		#pragma endregion

		#pragma region SETTINGS
		/**
		* A simple enumeration that define two actor spawn modes
		*/
		enum class EActorSpawnMode { ORIGIN, FRONT };

		/**
		* Defines if new actors should be spawned at origin
		* @param p_value
		*/
		void SetActorSpawnAtOrigin(bool p_value);

		/**
		* Defines how new actors should be spawned
		* @param p_value
		*/
		void SetActorSpawnMode(EActorSpawnMode p_value);

		/**
		* Reset the editor layout
		*/
		void ResetLayout();

		/**
		* Defines the scene view camera speed
		* @param p_speed
		*/
		void SetSceneViewCameraSpeed(int p_speed);

		/**
		* Returns the scene view camera speed
		*/
		int GetSceneViewCameraSpeed();

		/**
		* Defines the asset view camera speed
		* @param p_speed
		*/
		void SetAssetViewCameraSpeed(int p_speed);

		/**
		* Returns the asset view camera speed
		*/
		int GetAssetViewCameraSpeed();

		/**
		* Resets the scene view camera position to the default one
		*/
		void ResetSceneViewCameraPosition();

		/**
		* Resets the scene view camera position to the default one
		*/
		void ResetAssetViewCameraPosition();
		#pragma endregion

		#pragma region GAME
		/**
		* Defines some states for the editor
		*/
		enum class EEditorMode { EDIT, PLAY, PAUSE, FRAME_BY_FRAME };

		/**
		* Returns the current editor state/mode
		*/
		EEditorMode GetCurrentEditorMode() const;

		/**
		* Defines the editor state/mode
		* @param p_newEditorMode
		*/
		void SetEditorMode(EEditorMode p_newEditorMode);

		/**
		* Start playing the current scene and update the editor mode
		*/
		void StartPlaying();

		/**
		* Pause the current playing scene and update the editor mode
		*/
		void PauseGame();
		/**
		* Resume the current playing scene and update the editor mode
		*/
		void ResumeGame();

		/**
		* Stop platying the current scene and update the editor mode
		*/
		void StopPlaying();

		/**
		* Play the current frame and pause the editor
		*/
		void NextFrame();
		#pragma endregion

		#pragma region ACTOR_CREATION_DESTRUCTION
		/**
		* Create an actor with the given component type
		* @param p_focusOnCreation
		* @param p_parent
		*/
		template<typename T> ActorPtr CreateMonoComponentActor(bool p_focusOnCreation = true, ActorPtr p_parent = nullptr);

		/**
		* Calculate the position where to spawn the actor using the current camera position and forward
		* @param p_distanceToCamera
		*/
		LittleEngine::FVector3 CalculateActorSpawnPoint(float p_distanceToCamera);

		/**
		* Create an empty actor
		* @param p_focusOnCreation
		* @param p_parent
        * @param p_name
		*/
		ActorPtr	CreateEmptyActor(bool p_focusOnCreation = true, ActorPtr p_parent = nullptr, const std::string& p_name = "");

		/**
		* Create an actor with a model renderer and a material renderer. The model renderer with use the model identified
		* by the given path
		* @param p_path
		* @param p_focusOnCreation
		* @param p_parent
        * @param p_name
		*/
		ActorPtr	CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation = true, ActorPtr p_parent = nullptr, const std::string& p_name = "");

		/**
		* Destroy an actor from his scene
		* @param p_focusOnCreation
		* @param p_parent
		*/
		bool DestroyActor(ActorPtr p_actor);

		/**
		* Duplicate an actor
		* @param p_toDuplicate
		* @param p_forcedParent
		* @param bool
		*/
		void DuplicateActor(ActorPtr p_toDuplicate, ActorPtr p_forcedParent = nullptr, bool p_focus = true);
		#pragma endregion

		#pragma region ACTOR_MANIPULATION
		/**
		* Select an actor and show him in inspector
		* @param p_target
		*/
		void SelectActor(ActorPtr p_target);

		/**
		* Unselect any selected actor and clearing the inspector
		*/
		void UnselectActor();

		/**
		* Returns true if any actor is selected
		*/
		bool IsAnyActorSelected() const;

		/**
		* Returns the selected actor. Make sur you verified that an actor is selected
		* with IsAnyActorSelected() before calling this method
		*/
		ActorPtr		GetSelectedActor() const;

		/**
		* Moves the camera to the target actor
		*/
		void					MoveToTarget(ActorPtr p_target);
		/**
		* Select Asset
		*/
		void OnSelectAsset(LittleEngine::Utils::PathParser::EFileType p_type,std::string p_path,bool m_protected);
		/**
		* Unselect Asset 
		*/
		void OnUnselectAsset(std::string p_path);
		/**
		* Try to Open Asset 
		*/
		void OpenAssetByFileType(LittleEngine::Utils::PathParser::EFileType p_type,std::string p_path,bool m_protected);
		void OpenAssetMaterial(std::string p_path,bool m_protected);
		void OpenAssetScene(std::string p_path,bool m_protected);
		void OpenAssetShader(std::string p_path,bool m_protected);
		void OpenAssetTexture(std::string p_path,bool m_protected);
		void OpenAssetModel(std::string p_path,bool m_protected);
		template<typename TResource, typename TResourceLoader>
		void PreviewAsset(std::string p_path,bool m_protected);
		#pragma endregion

		#pragma region RESOURCE_MANAGEMENT
		/**
		* Compile every loaded shaders
		*/
		void CompileShaders();

		/**
		* Save every materials to their respective files
		*/
		void SaveMaterials();
		
		/**
		* Import an asset
		* @param p_initialDestinationDirectory
		*/
		bool ImportAsset(const std::string& p_initialDestinationDirectory);

		/**
		* Import an asset at location
		* @param p_destination
		*/
		bool ImportAssetAtLocation(const std::string& p_destination);

		/**
		* Returns the real path of a resource (complete absolute path)
		* @param p_path
		*/
		std::string GetRealPath(const std::string& p_path);

		/**
		* Returns the resource path of a file
		* @param p_path
		* @param p_isFromEngine
		*/
		std::string GetResourcePath(const std::string& p_path, bool p_isFromEngine = false);

		/**
		* Returns the script path of a file
		* @param p_path
		*/
		std::string GetScriptPath(const std::string& p_path);

		/**
		* Propagate the folder rename everywhere (Resource manager, scenes, materials...)
		* @param p_previousName
		* @param p_newName
		*/
		void PropagateFolderRename(std::string p_previousName, const std::string p_newName);

		/**
		* Propagate the folder destruction everywhere (Resource manager, scenes, materials...)
		* @param p_folderPath
		*/
		void PropagateFolderDestruction(std::string p_folderPath);

		/**
		* Propagate the script rename in scenes and inspector
		* @param p_previousName
		* @param p_newName
		*/
		void PropagateScriptRename(std::string p_previousName, std::string p_newName);

		/**
		* Propagate the file rename everywhere it is used
		* @param p_previousName
		* @param p_newName
		*/
		void PropagateFileRename(std::string p_previousName, std::string p_newName);

		/**
		* Propagate the file rename through concerned files
		* @param p_previousName
		* @param p_newName
		* @param p_fileType
		*/
		void PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName, const std::string& p_newName, LittleEngine::Utils::PathParser::EFileType p_fileType);
		#pragma endregion

		#pragma region SCENE
		/**
		* Load an empty scene. Any unsaved changes to the current scene will be discarded
		*/
		void LoadEmptyScene();

		/**
		* Save the current scene to the given path
		* @param p_path
		*/
		void SaveCurrentSceneTo(const std::string& p_path);

		/**
		* Load a scene from the disk
		* @param p_path
		* @param p_absolute
		*/
		void LoadSceneFromDisk(const std::string& p_path, bool p_absolute = false);
		/**
		* Load last scene from the disk
		*/
		void LoadLastScene();
		/**
		* Returns true if the current scene has been loaded from disk
		*/
		bool IsCurrentSceneLoadedFromDisk() const;

		/**
		* Save the current scene to its disk location
		*/
		void SaveSceneChanges();

		/**
		* Save the current scene to a new disk location (Can create a duplication of the scene file)
		*/
		void SaveAs();
		#pragma endregion

		#pragma region SCRIPTING
		/**
		* Refresh every scripts (Re-interpret)
		*/
		void RefreshScripts();
		#pragma endregion

		#pragma region BUILDING
		/**
		* Ask the user to select the build folder
		*/
		std::optional<std::string> SelectBuildFolder();

		/**
		* Build the current project
		* @param p_autoRun
		* @param p_tempFolder
		*/
		void Build(bool p_autoRun = false, bool p_tempFolder = false);

		/**
		* Build the current project at the given location
		* @param p_configuration
		* @param p_buildPath
		* @param p_autoRun
		*/
		void BuildAtLocation(const std::string& p_configuration, const std::string p_buildPath, bool p_autoRun = false);
		#pragma endregion

		#pragma region ACTION_SYSTEM
		/**
		* Prepare an action for a future call
		* @param p_action
		* @param p_frames
		*/
		void DelayAction(std::function<void()> p_action, uint32_t p_frames = 1);

		/**
		* Execute every actions that should be executed at this frame (Decrement the frame counter for each actions)
		*/
		void ExecuteDelayedActions();
		#pragma endregion

	public:
		LittleEngine::Eventing::Event<ActorPtr> ActorSelectedEvent;
		LittleEngine::Eventing::Event<ActorPtr> ActorUnselectedEvent;
		LittleEngine::Eventing::Event<EEditorMode> EditorModeChangedEvent;
		LittleEngine::Eventing::Event<> PlayEvent;

	private:
		Context& m_context;
		PanelsManager& m_panelsManager;
		EditorRenderer& m_renderer;

		EActorSpawnMode m_actorSpawnMode = EActorSpawnMode::ORIGIN;
		EEditorMode m_editorMode = EEditorMode::EDIT;

		std::vector<std::pair<uint32_t, std::function<void()>>> m_delayedActions;
		bool m_isLoadingScene = false;
		std::string m_sceneBackup;
	};
}

#include "../Editor/Core/EditorActions.inl"
