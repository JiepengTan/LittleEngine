/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Resource/Loader/AnimationLoader.h"

#include "assimp/postprocess.h"

#include "Resource/Asset/Animation.h"
#include "Resource/Loader/ModelLoader.h"
#include "Core/Tools/Filesystem/IniFile.h"
#include "fstream"
#include "iostream"
#include "sstream"

#include "Core/Maths/DebugUtil.h"


LittleEngine::Resources::EModelParserFlags GetAssetMetadataAA(const std::string& p_path)
{
	auto metaFile = LittleEngine::Filesystem::IniFile(p_path + ".meta");

	LittleEngine::Resources::EModelParserFlags flags = LittleEngine::Resources::EModelParserFlags::NONE;

	if (metaFile.GetOrDefault("CALC_TANGENT_SPACE",			true))	flags |= LittleEngine::Resources::EModelParserFlags::CALC_TANGENT_SPACE;
	if (metaFile.GetOrDefault("JOIN_IDENTICAL_VERTICES",	true))	flags |= LittleEngine::Resources::EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	if (metaFile.GetOrDefault("MAKE_LEFT_HANDED",			false))	flags |= LittleEngine::Resources::EModelParserFlags::MAKE_LEFT_HANDED;
	if (metaFile.GetOrDefault("TRIANGULATE",				true))	flags |= LittleEngine::Resources::EModelParserFlags::TRIANGULATE;
	if (metaFile.GetOrDefault("REMOVE_COMPONENT",			false))	flags |= LittleEngine::Resources::EModelParserFlags::REMOVE_COMPONENT;
	if (metaFile.GetOrDefault("GEN_NORMALS",				false))	flags |= LittleEngine::Resources::EModelParserFlags::GEN_NORMALS;
	if (metaFile.GetOrDefault("GEN_SMOOTH_NORMALS",			true))	flags |= LittleEngine::Resources::EModelParserFlags::GEN_SMOOTH_NORMALS;
	if (metaFile.GetOrDefault("SPLIT_LARGE_MESHES",			false))	flags |= LittleEngine::Resources::EModelParserFlags::SPLIT_LARGE_MESHES;
	if (metaFile.GetOrDefault("PRE_TRANSFORM_VERTICES",		true))	flags |= LittleEngine::Resources::EModelParserFlags::PRE_TRANSFORM_VERTICES;
	if (metaFile.GetOrDefault("LIMIT_BONE_WEIGHTS",			false))	flags |= LittleEngine::Resources::EModelParserFlags::LIMIT_BONE_WEIGHTS;
	if (metaFile.GetOrDefault("VALIDATE_DATA_STRUCTURE",	false))	flags |= LittleEngine::Resources::EModelParserFlags::VALIDATE_DATA_STRUCTURE;
	if (metaFile.GetOrDefault("IMPROVE_CACHE_LOCALITY",		true))	flags |= LittleEngine::Resources::EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	if (metaFile.GetOrDefault("REMOVE_REDUNDANT_MATERIALS", false))	flags |= LittleEngine::Resources::EModelParserFlags::REMOVE_REDUNDANT_MATERIALS;
	if (metaFile.GetOrDefault("FIX_INFACING_NORMALS",		false))	flags |= LittleEngine::Resources::EModelParserFlags::FIX_INFACING_NORMALS;
	if (metaFile.GetOrDefault("SORT_BY_PTYPE",				false))	flags |= LittleEngine::Resources::EModelParserFlags::SORT_BY_PTYPE;
	if (metaFile.GetOrDefault("FIND_DEGENERATES",			false))	flags |= LittleEngine::Resources::EModelParserFlags::FIND_DEGENERATES;
	if (metaFile.GetOrDefault("FIND_INVALID_DATA",			true))	flags |= LittleEngine::Resources::EModelParserFlags::FIND_INVALID_DATA;
	if (metaFile.GetOrDefault("GEN_UV_COORDS",				true))	flags |= LittleEngine::Resources::EModelParserFlags::GEN_UV_COORDS;
	if (metaFile.GetOrDefault("TRANSFORM_UV_COORDS",		false))	flags |= LittleEngine::Resources::EModelParserFlags::TRANSFORM_UV_COORDS;
	if (metaFile.GetOrDefault("FIND_INSTANCES",				true))	flags |= LittleEngine::Resources::EModelParserFlags::FIND_INSTANCES;
	if (metaFile.GetOrDefault("OPTIMIZE_MESHES",			true))	flags |= LittleEngine::Resources::EModelParserFlags::OPTIMIZE_MESHES;
	if (metaFile.GetOrDefault("OPTIMIZE_GRAPH",				true))	flags |= LittleEngine::Resources::EModelParserFlags::OPTIMIZE_GRAPH;
	if (metaFile.GetOrDefault("FLIP_UVS",					false))	flags |= LittleEngine::Resources::EModelParserFlags::FLIP_UVS;
	if (metaFile.GetOrDefault("FLIP_WINDING_ORDER",			false))	flags |= LittleEngine::Resources::EModelParserFlags::FLIP_WINDING_ORDER;
	if (metaFile.GetOrDefault("SPLIT_BY_BONE_COUNT",		false))	flags |= LittleEngine::Resources::EModelParserFlags::SPLIT_BY_BONE_COUNT;
	if (metaFile.GetOrDefault("DEBONE",						true))	flags |= LittleEngine::Resources::EModelParserFlags::DEBONE;
	if (metaFile.GetOrDefault("GLOBAL_SCALE",				true))	flags |= LittleEngine::Resources::EModelParserFlags::GLOBAL_SCALE;
	if (metaFile.GetOrDefault("EMBED_TEXTURES",				false))	flags |= LittleEngine::Resources::EModelParserFlags::EMBED_TEXTURES;
	if (metaFile.GetOrDefault("FORCE_GEN_NORMALS",			false))	flags |= LittleEngine::Resources::EModelParserFlags::FORCE_GEN_NORMALS;
	if (metaFile.GetOrDefault("DROP_NORMALS",				false))	flags |= LittleEngine::Resources::EModelParserFlags::DROP_NORMALS;
	if (metaFile.GetOrDefault("GEN_BOUNDING_BOXES",			false))	flags |= LittleEngine::Resources::EModelParserFlags::GEN_BOUNDING_BOXES;

	return { flags };
}


LittleEngine::Resources::AssimpParser LittleEngine::Resources::AnimationLoader::__ASSIMP;


LittleEngine::Animation* LittleEngine::Resources::AnimationLoader::Create(const std::string& p_filepath,LittleEngine::Model* p_model)
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
void LittleEngine::Resources::AnimationLoader::Reload(Animation& p_anim, const std::string& p_filePath,LittleEngine::Model* p_model)
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
bool LittleEngine::Resources::AnimationLoader::Destroy(Animation*& p_anim)
{
	if (p_anim)
	{
		delete p_anim;
		p_anim = nullptr;
		return true;
	}
	return false;
}

