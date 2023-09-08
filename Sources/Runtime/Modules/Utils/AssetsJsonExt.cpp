#include "AssetsJsonExt.h"
#include "Core/Serialization/JsonSerializer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Modules/Rendering/ResourceManagement/MaterialManager.h"
#include "Modules/Rendering/ResourceManagement/ModelManager.h"
#include "Modules/Rendering/ResourceManagement/ShaderManager.h"
#include "Modules/Rendering/ResourceManagement/SoundManager.h"
#include "Modules/Rendering/ResourceManagement/TextAssetManager.h"
#include "Modules/Rendering/ResourceManagement/TextureManager.h"

namespace LittleEngine
{
    template<>
    Json JsonSerializer::WriteRes(LittleEngine::Rendering::Resources::Model* instance){
        if(instance == nullptr) return Json("");
        return Json(instance->path);
    }
    template<>
    void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Rendering::Resources::Model*& instance)
    {
        assert(json_context.is_string());
        instance = GetGlobalService<ResourceManagement::ModelManager>().GetResource(json_context.string_value());
    }

    template<>
    Json JsonSerializer::WriteRes(LittleEngine::Rendering::Resources::Texture* instance)
    {
        if(instance == nullptr) return Json("");
        return Json(instance->path);
    }
    template<>
    void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Rendering::Resources::Texture*& instance)
    {
        assert(json_context.is_string());
        instance = GetGlobalService<ResourceManagement::TextureManager>().GetResource(json_context.string_value());
    }
    template<>
    Json JsonSerializer::WriteRes(LittleEngine::Rendering::Resources::Shader* instance)
    {
        if(instance == nullptr) return Json("");
        return Json(instance->path);
    }
    template<>
    void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Rendering::Resources::Shader*& instance)
    {
        assert(json_context.is_string());
        instance = GetGlobalService<ResourceManagement::ShaderManager>().GetResource(json_context.string_value());
    }
    template<>
    Json JsonSerializer::WriteRes(LittleEngine::Resources::Material* instance)
    {
        if(instance == nullptr) return Json("");
        return Json(instance->path);
    }
    template<>
    void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Resources::Material*& instance)
    {
        assert(json_context.is_string());
        instance = GetGlobalService<ResourceManagement::MaterialManager>().GetResource(json_context.string_value());
    }
    template<>
    Json JsonSerializer::WriteRes(LittleEngine::Audio::Resources::Sound* instance)
    {
        if(instance == nullptr) return Json("");
        return Json(instance->path);
    }
    template<>
    void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Audio::Resources::Sound*& instance)
    {
        assert(json_context.is_string());
        instance = GetGlobalService<ResourceManagement::SoundManager>().GetResource(json_context.string_value());
    }
    template<>
    Json JsonSerializer::WriteRes(LittleEngine::TextAsset* instance)
    {
        if(instance == nullptr) return Json("");
        return Json(instance->path);
    }
    template<>
    void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::TextAsset*& instance)
    {
        assert(json_context.is_string());
        instance = GetGlobalService<ResourceManagement::TextAssetManager>().GetResource(json_context.string_value());
    }
}
