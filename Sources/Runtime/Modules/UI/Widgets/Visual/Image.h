/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Core/Maths/FVector2.h"

#include "Modules/UI/Internal/TextureID.h"
#include "Modules/UI/Widgets/AWidget.h"

namespace LittleEngine::UI::Widgets::Visual
{
	/**
	* Simple widget that display an image
	*/
	class Image : public AWidget
	{
	public:
		/**
		* Constructor
		* @param p_textureID
		* @parma p_size
		*/
		Image(uint32_t p_textureID, const LittleEngine::FVector2& p_size);

	protected:
		void _Draw_Impl() override;

	public:
		Internal::TextureID textureID;
		std::string guid;
		LittleEngine::FVector2 size;
	};
}
