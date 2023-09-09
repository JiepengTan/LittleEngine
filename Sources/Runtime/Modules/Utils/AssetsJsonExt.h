#pragma once
#include "Core/Serialization/JsonSerializer.h"
#include "Resource/Asset/Model.h"
#include "Resource/Asset/Texture.h"
#include "Resource/Asset/Shader.h"
#include "Resource/Asset/Material.h"
#include "Resource/Asset/Sound.h"
#include "Resource/Asset/TextAsset.h"

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
