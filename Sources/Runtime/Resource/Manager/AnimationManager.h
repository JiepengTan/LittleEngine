/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Modules/Rendering/ResourceManagement/AResourceManager.h"
#include "Modules/Rendering/Resources/Animation.h"

namespace LittleEngine::ResourceManagement
{
    /**
    * ResourceManager of materials
    */
    class AnimationManager : public AResourceManager<LittleEngine::Rendering::Resources::Animation>
    {
    public:
        /**
        * Create the resource identified by the given path
        * @param p_path
        */
        virtual LittleEngine::Rendering::Resources::Animation* CreateResource(const std::string & p_path) override;

        /**
        * Destroy the given resource
        * @param p_resource
        */
        virtual void DestroyResource(LittleEngine::Rendering::Resources::Animation* p_resource) override;

        /**
        * Reload the given resource
        * @param p_resource
        * @param p_path
        */
        virtual void ReloadResource(LittleEngine::Rendering::Resources::Animation* p_resource, const std::string& p_path) override;

    public:
        LittleEngine::Rendering::Resources::Model* currentModel;
    };
}
