/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Tools/Filesystem/IniFile.h"

#include "Modules/Rendering/Buffers/UniformBuffer.h"
#include "Modules/Rendering/Buffers/ShaderStorageBuffer.h"
#include "Modules/Rendering/Core/ShapeDrawer.h"

#include "Modules/Physics/Core/PhysicsEngine.h"

#include "Modules/Audio/Core/AudioEngine.h"

#include "Platform/Windowing/Context/Device.h"
#include "Platform/Windowing/Inputs/InputManager.h"
#include "Platform/Windowing/Window.h"

#include "Modules/Framework/ECS/SceneRenderer.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/SoundManager.h"
#include "Modules/Rendering/ResourceManagement/AnimationManager.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"
#include "Modules/Scripting/ScriptInterpreter.h"

#include "Modules/UI/Core/UIManager.h"
#include "Modules/Audio/Core/AudioEngine.h"
#include "Modules/Audio/Core/AudioPlayer.h"

#include "../Editor/Core/EditorResources.h"

namespace LittleEditor::Core
{
	/**
	* The Context handle the engine features setup
	*/
	class Context
	{
	public:
		/**
		* Constructor
		* @param p_projectPath
		* @param p_projectName
		*/
		Context(const std::string& p_projectPath, const std::string& p_projectName);

		/**
		* Destructor
		*/
		~Context();

		/**
		* Reset project settings ini file
		*/
		void ResetProjectSettings();

		/**
		* Verify that project settings are complete (No missing key).
		* Returns true if the integrity is verified
		*/
		bool IsProjectSettingsIntegrityVerified();

		/**
		* Apply project settings to the ini file
		*/
		void ApplyProjectSettings();

	public:
		const std::string projectPath;
		const std::string projectName;
		const std::string projectFilePath;
		const std::string engineAssetsPath;
		const std::string projectAssetsPath;
		const std::string projectScriptsPath;
		const std::string editorAssetsPath;

		std::unique_ptr<LittleEngine::Windowing::Context::Device>			device;
		std::unique_ptr<LittleEngine::Windowing::Window>					window;
		std::unique_ptr<LittleEngine::Windowing::Inputs::InputManager>		inputManager;
		std::unique_ptr<LittleEngine::Rendering::Context::Driver>			driver;
		std::unique_ptr<LittleEngine::SceneRenderer>					renderer;
		std::unique_ptr<LittleEngine::Rendering::Core::ShapeDrawer>			shapeDrawer;
		std::unique_ptr<LittleEngine::UI::Core::UIManager>					uiManager;
		std::unique_ptr<LittleEngine::Physics::Core::PhysicsEngine>			physicsEngine;
		std::unique_ptr<LittleEngine::Audio::Core::AudioEngine>				audioEngine;
		std::unique_ptr<LittleEngine::Audio::Core::AudioPlayer>				audioPlayer;
		std::unique_ptr<LittleEditor::Core::EditorResources>		editorResources;

		std::unique_ptr<LittleEngine::Scripting::ScriptInterpreter>	scriptInterpreter;
		std::unique_ptr<LittleEngine::Rendering::Buffers::UniformBuffer>	engineUBO;

		std::unique_ptr<LittleEngine::Rendering::Buffers::ShaderStorageBuffer>	lightSSBO;
		std::unique_ptr<LittleEngine::Rendering::Buffers::ShaderStorageBuffer>	simulatedLightSSBO;
		
		LittleEngine::SceneSystem::SceneManager sceneManager;

		LittleEngine::ResourceManagement::ModelManager	modelManager;
		LittleEngine::ResourceManagement::TextureManager	textureManager;
		LittleEngine::ResourceManagement::ShaderManager	shaderManager;
		LittleEngine::ResourceManagement::MaterialManager	materialManager;
		LittleEngine::ResourceManagement::SoundManager	soundManager;
		LittleEngine::ResourceManagement::AnimationManager animationManager;

		LittleEngine::Windowing::Settings::WindowSettings windowSettings;

		LittleEngine::Filesystem::IniFile projectSettings;
		LittleEngine::Filesystem::IniFile editSettings;
	};
}
