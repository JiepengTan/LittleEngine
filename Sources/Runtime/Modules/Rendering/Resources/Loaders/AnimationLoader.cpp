/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/Rendering/Resources/Loaders/AnimationLoader.h"

#include <assimp/postprocess.h>

#include "Modules/Rendering/Resources/Animation.h"
#include "Modules/Rendering/Resources/Loaders/ModelLoader.h"
#include "Core/Tools/Filesystem/IniFile.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "Core/Maths/DebugUtil.h"


OvRendering::Resources::Parsers::EModelParserFlags GetAssetMetadataAA(const std::string& p_path)
{
	auto metaFile = OvTools::Filesystem::IniFile(p_path + ".meta");

	OvRendering::Resources::Parsers::EModelParserFlags flags = OvRendering::Resources::Parsers::EModelParserFlags::NONE;

	if (metaFile.GetOrDefault("CALC_TANGENT_SPACE",			true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::CALC_TANGENT_SPACE;
	if (metaFile.GetOrDefault("JOIN_IDENTICAL_VERTICES",	true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	if (metaFile.GetOrDefault("MAKE_LEFT_HANDED",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::MAKE_LEFT_HANDED;
	if (metaFile.GetOrDefault("TRIANGULATE",				true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::TRIANGULATE;
	if (metaFile.GetOrDefault("REMOVE_COMPONENT",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::REMOVE_COMPONENT;
	if (metaFile.GetOrDefault("GEN_NORMALS",				false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GEN_NORMALS;
	if (metaFile.GetOrDefault("GEN_SMOOTH_NORMALS",			true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GEN_SMOOTH_NORMALS;
	if (metaFile.GetOrDefault("SPLIT_LARGE_MESHES",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::SPLIT_LARGE_MESHES;
	if (metaFile.GetOrDefault("PRE_TRANSFORM_VERTICES",		true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	if (metaFile.GetOrDefault("LIMIT_BONE_WEIGHTS",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::LIMIT_BONE_WEIGHTS;
	if (metaFile.GetOrDefault("VALIDATE_DATA_STRUCTURE",	false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::VALIDATE_DATA_STRUCTURE;
	if (metaFile.GetOrDefault("IMPROVE_CACHE_LOCALITY",		true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	if (metaFile.GetOrDefault("REMOVE_REDUNDANT_MATERIALS", false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::REMOVE_REDUNDANT_MATERIALS;
	if (metaFile.GetOrDefault("FIX_INFACING_NORMALS",		false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FIX_INFACING_NORMALS;
	if (metaFile.GetOrDefault("SORT_BY_PTYPE",				false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::SORT_BY_PTYPE;
	if (metaFile.GetOrDefault("FIND_DEGENERATES",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FIND_DEGENERATES;
	if (metaFile.GetOrDefault("FIND_INVALID_DATA",			true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FIND_INVALID_DATA;
	if (metaFile.GetOrDefault("GEN_UV_COORDS",				true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GEN_UV_COORDS;
	if (metaFile.GetOrDefault("TRANSFORM_UV_COORDS",		false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::TRANSFORM_UV_COORDS;
	if (metaFile.GetOrDefault("FIND_INSTANCES",				true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FIND_INSTANCES;
	if (metaFile.GetOrDefault("OPTIMIZE_MESHES",			true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_MESHES;
	if (metaFile.GetOrDefault("OPTIMIZE_GRAPH",				true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::OPTIMIZE_GRAPH;
	if (metaFile.GetOrDefault("FLIP_UVS",					false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FLIP_UVS;
	if (metaFile.GetOrDefault("FLIP_WINDING_ORDER",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FLIP_WINDING_ORDER;
	if (metaFile.GetOrDefault("SPLIT_BY_BONE_COUNT",		false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::SPLIT_BY_BONE_COUNT;
	if (metaFile.GetOrDefault("DEBONE",						true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::DEBONE;
	if (metaFile.GetOrDefault("GLOBAL_SCALE",				true))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GLOBAL_SCALE;
	if (metaFile.GetOrDefault("EMBED_TEXTURES",				false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::EMBED_TEXTURES;
	if (metaFile.GetOrDefault("FORCE_GEN_NORMALS",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::FORCE_GEN_NORMALS;
	if (metaFile.GetOrDefault("DROP_NORMALS",				false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::DROP_NORMALS;
	if (metaFile.GetOrDefault("GEN_BOUNDING_BOXES",			false))	flags |= OvRendering::Resources::Parsers::EModelParserFlags::GEN_BOUNDING_BOXES;

	return { flags };
}


OvRendering::Resources::Parsers::AssimpParser OvRendering::Resources::Loaders::AnimationLoader::__ASSIMP;


OvRendering::Resources::Animation* OvRendering::Resources::Loaders::AnimationLoader::Create(const std::string& p_filepath,OvRendering::Resources::Model* p_model)
{
	Animation* result = new Animation();
	result->path = p_filepath;
	// The boneId of the animation should be the same as the boneId of the model
	result->m_name2BoneInfo = &p_model->GetBoneInfoMap();
	result->m_BoneCounter = &p_model->GetBoneCount();
	auto flags = GetAssetMetadataAA(p_filepath);
	if (__ASSIMP.LoadAnimation(result,p_filepath,flags))
	{
		return result;
	}
	delete result;
	return nullptr;
}
void OvRendering::Resources::Loaders::AnimationLoader::Reload(Animation& p_anim, const std::string& p_filePath,OvRendering::Resources::Model* p_model)
{
	Animation* newModel = Create(p_filePath,p_model);
	if (newModel)
	{
		p_anim.path = newModel->path;
		p_anim.m_Bones =std::move(newModel->m_Bones);
		p_anim.m_Duration = newModel->m_Duration;
		p_anim.m_TicksPerSecond = newModel->m_TicksPerSecond;
		delete newModel;
	}
}
bool OvRendering::Resources::Loaders::AnimationLoader::Destroy(Animation*& p_anim)
{
	if (p_anim)
	{
		delete p_anim;
		p_anim = nullptr;
		return true;
	}
	return false;
}

