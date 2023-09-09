#pragma once

#include "string"

#include "Resource/Asset/Model.h"
#include "Resource/Parser/AssimpParser.h"

namespace LittleEngine::Resources
{
	class AnimationLoader
	{
	public:
		static Animation* Create(const std::string& p_filepath,LittleEngine::Model* p_model);
		static void Reload(Animation& p_anim, const std::string& p_filePath,LittleEngine::Model* p_model);
		static bool Destroy(Animation*& p_anim);
	private:
		static AssimpParser __ASSIMP;
	};
}
