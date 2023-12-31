/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/Manager/AResourceManager.h"
#include "Resource/Asset/Animation.h"

namespace LittleEngine::ResourceManagement
{
    /**
    * ResourceManager of materials
    */
    class AnimationManager : public AResourceManager<LittleEngine::Animation>
    {
    public:
        /**
        * Create the resource identified by the given path
        * @param p_path
        */
        virtual LittleEngine::Animation* CreateResource(const std::string & p_path) override;

        /**
        * Destroy the given resource
        * @param p_resource
        */
        virtual void DestroyResource(LittleEngine::Animation* p_resource) override;

        /**
        * Reload the given resource
        * @param p_resource
        * @param p_path
        */
        virtual void ReloadResource(LittleEngine::Animation* p_resource, const std::string& p_path) override;

    public:
        LittleEngine::Model* currentModel;
    };
}
