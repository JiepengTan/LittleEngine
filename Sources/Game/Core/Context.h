/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/Buffers/UniformBuffer.h"
#include "Modules/Rendering/Buffers/ShaderStorageBuffer.h"

#include "Modules/Physics/Core/PhysicsEngine.h"

#include "Platform/Windowing/Context/Device.h"
#include "Platform/Windowing/Inputs/InputManager.h"
#include "Platform/Windowing/Window.h"

#include "Modules/UI/Core/UIManager.h"

#include "Modules/Framework/ECS/SceneRenderer.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/SoundManager.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"
#include "Modules/Scripting/ScriptInterpreter.h"

#include "Modules/Audio/Core/AudioEngine.h"
#include "Modules/Audio/Core/AudioPlayer.h"

#include "Core/Tools/Filesystem/IniFile.h"
#include "Modules/Rendering/ResourceManagement/TextAssetManager.h"

namespace LittleEngine::ResourceManagement
{
	class TextAssetManager;
}

namespace LittleGame::Core
{
	/**
	* The Context handle the engine features setup
	*/
	class Context
	{
	public:
		/**
		* Constructor
		*/
		Context();

		/**
		* Destructor
		*/
		~Context();

	public:
		const std::string engineAssetsPath;
		const std::string projectAssetsPath;
		const std::string projectScriptsPath;

		std::unique_ptr<LittleEngine::Windowing::Context::Device>				device;
		std::unique_ptr<LittleEngine::Windowing::Window>						window;
		std::unique_ptr<LittleEngine::Windowing::Inputs::InputManager>			inputManager;
		std::unique_ptr<LittleEngine::Rendering::Context::Driver>				driver;
		std::unique_ptr<LittleEngine::SceneRenderer>						renderer;
		std::unique_ptr<LittleEngine::UI::Core::UIManager>						uiManager;
		std::unique_ptr<LittleEngine::Physics::Core::PhysicsEngine>				physicsEngine;
		std::unique_ptr<LittleEngine::Audio::Core::AudioEngine>					audioEngine;
		std::unique_ptr<LittleEngine::Audio::Core::AudioPlayer>					audioPlayer;
		std::unique_ptr<LittleEngine::Scripting::ScriptInterpreter>		scriptInterpreter;
		std::unique_ptr<LittleEngine::Rendering::Buffers::UniformBuffer>		engineUBO;
		std::unique_ptr<LittleEngine::Rendering::Buffers::ShaderStorageBuffer>	lightSSBO;

		LittleEngine::SceneManager sceneManager;

		LittleEngine::ResourceManagement::ModelManager	modelManager;
		LittleEngine::ResourceManagement::TextureManager	textureManager;
		LittleEngine::ResourceManagement::ShaderManager	shaderManager;
		LittleEngine::ResourceManagement::MaterialManager	materialManager;
		LittleEngine::ResourceManagement::SoundManager	soundManager;
		LittleEngine::ResourceManagement::TextAssetManager	textAssetManager;
		
		LittleEngine::Filesystem::IniFile projectSettings;
	};
}
