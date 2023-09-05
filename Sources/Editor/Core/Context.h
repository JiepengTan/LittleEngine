/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Tools/Filesystem/IniFile.h"

#include "Resource/Rendering/Buffers/UniformBuffer.h"
#include "Resource/Rendering/Buffers/ShaderStorageBuffer.h"
#include "Resource/Rendering/Core/ShapeDrawer.h"

#include "Modules/Physics/Core/PhysicsEngine.h"

#include "Modules/Audio/Core/AudioEngine.h"

#include "Platform/Windowing/Context/Device.h"
#include "Platform/Windowing/Inputs/InputManager.h"
#include "Platform/Windowing/Window.h"

#include "Modules/Framework/ECS/SceneRenderer.h"
#include "Resource/ResourceManagement/ModelManager.h"
#include "Resource/ResourceManagement/TextureManager.h"
#include "Resource/ResourceManagement/ShaderManager.h"
#include "Resource/ResourceManagement/MaterialManager.h"
#include "Resource/ResourceManagement/SoundManager.h"
#include "Resource/ResourceManagement/AnimationManager.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"
#include "Modules/Scripting/ScriptInterpreter.h"

#include "Modules/UI/Core/UIManager.h"
#include "Modules/Audio/Core/AudioEngine.h"
#include "Modules/Audio/Core/AudioPlayer.h"

#include "../Editor/Core/EditorResources.h"

namespace OvEditor::Core
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

		std::unique_ptr<OvWindowing::Context::Device>			device;
		std::unique_ptr<OvWindowing::Window>					window;
		std::unique_ptr<OvWindowing::Inputs::InputManager>		inputManager;
		std::unique_ptr<OvRendering::Context::Driver>			driver;
		std::unique_ptr<OvCore::ECS::SceneRenderer>					renderer;
		std::unique_ptr<OvRendering::Core::ShapeDrawer>			shapeDrawer;
		std::unique_ptr<OvUI::Core::UIManager>					uiManager;
		std::unique_ptr<OvPhysics::Core::PhysicsEngine>			physicsEngine;
		std::unique_ptr<OvAudio::Core::AudioEngine>				audioEngine;
		std::unique_ptr<OvAudio::Core::AudioPlayer>				audioPlayer;
		std::unique_ptr<OvEditor::Core::EditorResources>		editorResources;

		std::unique_ptr<OvCore::Scripting::ScriptInterpreter>	scriptInterpreter;
		std::unique_ptr<OvRendering::Buffers::UniformBuffer>	engineUBO;

		std::unique_ptr<OvRendering::Buffers::ShaderStorageBuffer>	lightSSBO;
		std::unique_ptr<OvRendering::Buffers::ShaderStorageBuffer>	simulatedLightSSBO;
		
		OvCore::SceneSystem::SceneManager sceneManager;

		OvCore::ResourceManagement::ModelManager	modelManager;
		OvCore::ResourceManagement::TextureManager	textureManager;
		OvCore::ResourceManagement::ShaderManager	shaderManager;
		OvCore::ResourceManagement::MaterialManager	materialManager;
		OvCore::ResourceManagement::SoundManager	soundManager;
		OvCore::ResourceManagement::AnimationManager animationManager;

		OvWindowing::Settings::WindowSettings windowSettings;

		OvTools::Filesystem::IniFile projectSettings;
		OvTools::Filesystem::IniFile editSettings;
	};
}
