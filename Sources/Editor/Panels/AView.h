/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "Modules/UI/Panels/PanelWindow.h"
#include "Modules/UI/Widgets/Visual/Image.h"
#include "Modules/Rendering/Buffers/UniformBuffer.h"
#include "Modules/Rendering/Buffers/Framebuffer.h"
#include "Modules/Rendering/LowRenderer/Camera.h"

namespace LittleEditor::Core { class EditorRenderer; }

namespace LittleEditor::Panels
{
	/**
	* Base class for any view
	*/
	class AView : public LittleEngine::UI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		AView
		(
			const std::string& p_title,
			bool p_opened,
			const LittleEngine::UI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the view
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime);

		/**
		* Custom implementation of the draw method
		*/
		void _Draw_Impl() override;

		/**
		* Custom implementation of the render method to define in dervied classes
		*/
		virtual void _Render_Impl() = 0;

		/**
		* Render the view
		*/
		void Render();

		/**
		* Defines the camera position
		* @param p_position
		*/
		void SetCameraPosition(const LittleEngine::FVector3& p_position);

		/**
		* Defines the camera rotation
		* @param p_rotation
		*/
		void SetCameraRotation(const LittleEngine::FQuaternion& p_rotation);

		/**
		* Returns the camera position
		*/
		const LittleEngine::FVector3& GetCameraPosition() const;

		/**
		* Returns the camera rotation
		*/
		const LittleEngine::FQuaternion& GetCameraRotation() const;

		/**
		* Returns the camera used by this view
		*/
		LittleEngine::Rendering::LowRenderer::Camera& GetCamera();

		/**
		* Returns the size of the panel ignoring its titlebar height
		*/
		std::pair<uint16_t, uint16_t> GetSafeSize() const;

		/**
		* Returns the grid color of the view
		*/
		const LittleEngine::FVector3& GetGridColor() const;

		/**
		* Defines the grid color of the view
		* @param p_color
		*/
		void SetGridColor(const LittleEngine::FVector3& p_color);

		/**
		* Fill the UBO using the view settings
		*/
		void FillEngineUBO();

	protected:
		/**
		* Update camera matrices
		*/
		void PrepareCamera();

	protected:
		LittleEditor::Core::EditorRenderer& m_editorRenderer;
		LittleEngine::Rendering::LowRenderer::Camera m_camera;
		LittleEngine::FVector3 m_cameraPosition;
		LittleEngine::FQuaternion m_cameraRotation;
		LittleEngine::UI::Widgets::Visual::Image* m_image;

        LittleEngine::FVector3 m_gridColor = LittleEngine::FVector3 { 0.176f, 0.176f, 0.176f };

		LittleEngine::Rendering::Buffers::Framebuffer m_fbo;
	};
}
