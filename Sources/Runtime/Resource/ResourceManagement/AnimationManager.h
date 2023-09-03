/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "Resource/ResourceManagement/AResourceManager.h"
#include "Resource/Rendering/Resources/Animation.h"

namespace OvCore::ResourceManagement
{
    /**
    * ResourceManager of materials
    */
    class AnimationManager : public AResourceManager<OvRendering::Resources::Animation>
    {
    public:
        /**
        * Create the resource identified by the given path
        * @param p_path
        */
        virtual OvRendering::Resources::Animation* CreateResource(const std::string & p_path) override;

        /**
        * Destroy the given resource
        * @param p_resource
        */
        virtual void DestroyResource(OvRendering::Resources::Animation* p_resource) override;

        /**
        * Reload the given resource
        * @param p_resource
        * @param p_path
        */
        virtual void ReloadResource(OvRendering::Resources::Animation* p_resource, const std::string& p_path) override;

    public:
        OvRendering::Resources::Model* currentModel;
    };
}
