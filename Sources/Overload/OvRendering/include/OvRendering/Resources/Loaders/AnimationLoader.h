#pragma once

#include <string>

#include "OvRendering/Resources/Model.h"
#include "OvRendering/Resources/Parsers/AssimpParser.h"

namespace OvRendering::Resources::Loaders
{
	class AnimationLoader
	{
	public:
		static Animation* Create(const std::string& p_filepath,OvRendering::Resources::Model* p_model);
		static void Reload(Animation& p_anim, const std::string& p_filePath,OvRendering::Resources::Model* p_model);
		static bool Destroy(Animation*& p_anim);
	private:
		static Parsers::AssimpParser __ASSIMP;
	};
}