/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/FVector2.h"

#include "Modules/UI/Internal/TextureID.h"
#include "Modules/UI/Types/Color.h"
#include "Modules/UI/Widgets/Buttons/AButton.h"

namespace LittleEngine::UI::Widgets::Buttons
{
	/**
	* Button widget with an image
	*/
	class ButtonImage : public AButton
	{
	public:
		/**
		* Constructor
		* @param p_textureID
		* @param p_size
		*/
		ButtonImage(uint32_t p_textureID, const LittleEngine::FVector2& p_size);

	protected:
		void _Draw_Impl() override;

	public:
		bool disabled = false;

		LittleEngine::UI::Types::Color background = { 0, 0, 0, 0 };
		LittleEngine::UI::Types::Color tint = { 1, 1, 1, 1 };

		Internal::TextureID textureID;
		LittleEngine::FVector2 size;
		bool isAbsoluteOffset = false;
		LittleEngine::FVector2 offset;
		
	};
}
