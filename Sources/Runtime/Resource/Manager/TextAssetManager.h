#pragma once


#include "Core/Serialization/Json.h"
#include "Modules/Rendering/ResourceManagement/AResourceManager.h"
#include "Resource/TextAsset.h"

namespace LittleEngine::ResourceManagement
{
    
    /**
    * ResourceManager of materials
    */
    class TextAssetManager : public AResourceManager<TextAsset>
    {
    public:
        /**
        * Create the resource identified by the given path
        * @param p_path
        */
        virtual TextAsset* CreateResource(const std::string & p_path) override;

        /**
        * Destroy the given resource
        * @param p_resource
        */
        virtual void DestroyResource(TextAsset* p_resource) override;

        /**
        * Reload the given resource
        * @param p_resource
        * @param p_path
        */
        virtual void ReloadResource(TextAsset* p_resource, const std::string& p_path) override;

    };
}
