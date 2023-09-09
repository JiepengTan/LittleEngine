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
  Json JsonSerializer::WriteRes(LittleEngine::Model* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Model*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Texture* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Texture*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Shader* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Shader*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Material* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Material*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::Sound* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::Sound*& instance);
  template<>
  Json JsonSerializer::WriteRes(LittleEngine::TextAsset* instance);
  template<>
  void JsonSerializer::ReadRes(const Json& json_context, LittleEngine::TextAsset*& instance);
}
