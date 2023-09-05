/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Framework/ECS/Actor.h"
#include "Modules/Framework/SceneSystem/SceneManager.h"
#include "Modules/Framework/ECS/Components/CCamera.h"

#include "../Game/Core/Context.h"

namespace LittleGame::Core
{
	/**
	* Handle the rendering of the game scene
	*/
	class GameRenderer
	{
	public:
		/**
		* Create the EditorRenderer
		* @param p_context
		*/
		GameRenderer(Context& p_context);

		/**
		* Render the scene
		*/
		void RenderScene();

		/**
		* Update the engine UBO
		* @param p_mainCamera
		*/
		void UpdateEngineUBO(LittleEngine::CCamera& p_mainCamera);

		/**
		* Update the light SSBO with the current scene
		* @param p_scene
		*/
		void UpdateLights(LittleEngine::SceneSystem::Scene& p_scene);

		/**
		* Update the light SSBO with the current scene (Lights outside of the given frustum are culled)
		* @param p_scene
		*/
		void UpdateLightsInFrustum(LittleEngine::SceneSystem::Scene& p_scene, const LittleEngine::Rendering::Data::Frustum& p_frustum);

	private:
		Context& m_context;
		LittleEngine::Resources::Material m_emptyMaterial;
	};
}
