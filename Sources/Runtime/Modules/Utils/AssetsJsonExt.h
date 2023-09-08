#pragma once
#include "Core/Serialization/JsonSerializer.h"
#include "Modules/Rendering/Resources/Model.h"
#include "Modules/Rendering/Resources/Texture.h"
#include "Modules/Rendering/Resources/Shader.h"
#include "Modules/Rendering/Resources/Material.h"
#include "Modules/Audio/Resources/Sound.h"
#include "Resource/TextAsset.h"

namespace LittleEngine
{
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Rendering::Resources::Model* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Rendering::Resources::Model*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Rendering::Resources::Texture* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Rendering::Resources::Texture*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Rendering::Resources::Shader* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Rendering::Resources::Shader*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Resources::Material* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Resources::Material*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Audio::Resources::Sound* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Audio::Resources::Sound*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::TextAsset* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::TextAsset*& instance);
}