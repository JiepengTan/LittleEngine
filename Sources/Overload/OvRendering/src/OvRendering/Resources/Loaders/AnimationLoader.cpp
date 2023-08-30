/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Resources/Loaders/AnimationLoader.h"
#include "OvRendering/Resources/Animation.h"
#include "OvRendering/Resources/Loaders/ModelLoader.h"

OvRendering::Resources::Parsers::AssimpParser OvRendering::Resources::Loaders::AnimationLoader::__ASSIMP;

OvRendering::Resources::Animation* OvRendering::Resources::Loaders::AnimationLoader::Create(const std::string& p_filepath,OvRendering::Resources::Model* p_model)
{
	Animation* result = new Animation();
	result->m_path = p_filepath;
	// The boneId of the animation should be the same as the boneId of the model
	result->m_name2BoneInfo = p_model->GetBoneInfoMap();
	result->m_BoneCounter = p_model->GetBoneCount();
	
	if (__ASSIMP.LoadAnimation(result,p_filepath))
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
		p_anim.m_path = newModel->m_path;
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

