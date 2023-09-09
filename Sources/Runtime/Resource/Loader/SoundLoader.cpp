/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Loader//SoundLoader.h"

LittleEngine::Sound* LittleEngine::Resources::SoundLoader::Create(const std::string& p_filepath)
{
	return new Sound(p_filepath);
}

void LittleEngine::Resources::SoundLoader::Reload(Sound& p_sound, const std::string& p_path)
{
	*const_cast<std::string*>(&p_sound.path) = p_path;
}

bool LittleEngine::Resources::SoundLoader::Destroy(Sound*& p_soundInstance)
{
	if (p_soundInstance)
	{
		delete p_soundInstance;
		p_soundInstance = nullptr;

		return true;
	}

	return false;
}
