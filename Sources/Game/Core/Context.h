/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/Rendering/Buffers/UniformBuffer.h"
#include "Resource/Rendering/Buffers/ShaderStorageBuffer.h"

#include "Modules/Physics/Core/PhysicsEngine.h"

#include "Platform/Windowing/Context/Device.h"
#include "Platform/Windowing/Inputs/InputManager.h"
#include "Platform/Windowing/Window.h"

#include "Modules/UI/Core/UIManager.h"

#include "GamePlay/ECS/Renderer.h"
#include "Resource/ResourceManagement/ModelManager.h"
#include "Resource/ResourceManagement/TextureManager.h"
#include "Resource/ResourceManagement/ShaderManager.h"
#include "Resource/ResourceManagement/MaterialManager.h"
#include "Resource/ResourceManagement/SoundManager.h"
#include "GamePlay/SceneSystem/SceneManager.h"
#include "GamePlay/Scripting/ScriptInterpreter.h"

#include "Modules/Audio/Core/AudioEngine.h"
#include "Modules/Audio/Core/AudioPlayer.h"

#include "Core/Tools/Filesystem/IniFile.h"

namespace OvGame::Core
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

		std::unique_ptr<OvWindowing::Context::Device>				device;
		std::unique_ptr<OvWindowing::Window>						window;
		std::unique_ptr<OvWindowing::Inputs::InputManager>			inputManager;
		std::unique_ptr<OvRendering::Context::Driver>				driver;
		std::unique_ptr<OvCore::ECS::Renderer>						renderer;
		std::unique_ptr<OvUI::Core::UIManager>						uiManager;
		std::unique_ptr<OvPhysics::Core::PhysicsEngine>				physicsEngine;
		std::unique_ptr<OvAudio::Core::AudioEngine>					audioEngine;
		std::unique_ptr<OvAudio::Core::AudioPlayer>					audioPlayer;
		std::unique_ptr<OvCore::Scripting::ScriptInterpreter>		scriptInterpreter;
		std::unique_ptr<OvRendering::Buffers::UniformBuffer>		engineUBO;
		std::unique_ptr<OvRendering::Buffers::ShaderStorageBuffer>	lightSSBO;

		OvCore::SceneSystem::SceneManager sceneManager;

		OvCore::ResourceManagement::ModelManager	modelManager;
		OvCore::ResourceManagement::TextureManager	textureManager;
		OvCore::ResourceManagement::ShaderManager	shaderManager;
		OvCore::ResourceManagement::MaterialManager	materialManager;
		OvCore::ResourceManagement::SoundManager	soundManager;
		
		OvTools::Filesystem::IniFile projectSettings;
	};
}
