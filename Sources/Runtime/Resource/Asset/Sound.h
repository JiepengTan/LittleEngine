/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "string"



namespace LittleEngine
{
	namespace Resources { class SoundLoader; }

	/**
	* Playable sound
	*/
	class Sound
	{
		friend class Resources::SoundLoader;

	private:
		Sound(const std::string& p_path);

	public:
		const std::string path;
	};
}
