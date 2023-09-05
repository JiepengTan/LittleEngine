#pragma once

#include <string>

#include "Modules/Rendering/Resources/Model.h"
#include "Modules/Rendering/Resources/Parsers/AssimpParser.h"

namespace LittleEngine::Rendering::Resources::Loaders
{
	class AnimationLoader
	{
	public:
		static Animation* Create(const std::string& p_filepath,LittleEngine::Rendering::Resources::Model* p_model);
		static void Reload(Animation& p_anim, const std::string& p_filePath,LittleEngine::Rendering::Resources::Model* p_model);
		static bool Destroy(Animation*& p_anim);
	private:
		static Parsers::AssimpParser __ASSIMP;
	};
}
