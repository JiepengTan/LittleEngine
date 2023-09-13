#include "Resource/Core/ResourcesUtils.h"

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/AnimationManager.h"
#include "Resource/Manager/MaterialManager.h"
#include "Resource/Manager/ModelManager.h"
#include "Resource/Manager/ShaderManager.h"
#include "Resource/Manager/SoundManager.h"
#include "Resource/Manager/TextureManager.h"
#include "Resource/Core/ResPtr.h"

namespace LittleEngine
{
    void ResourcesUtils::DoNothing(){}
    
#define DEFINE_RES_LOAD_FUNCTION(restypename)\
    void ResourcesUtils::LoadRes(const StringText& guid, restypename*& ptr,bool p_tryToLoadIfNotFound){\
        ptr = GetGlobalService<ResourceManagement::##restypename##Manager>().GetResource(guid,p_tryToLoadIfNotFound);\
    }\
    restypename* ResourcesUtils::Load##restypename(const StringText& guid, bool p_tryToLoadIfNotFound ){\
        return GetGlobalService<ResourceManagement::##restypename##Manager>().GetResource(guid,p_tryToLoadIfNotFound);\
    }\
    restypename##ResPtr ResourcesUtils::Load##restypename##ResPtr(const StringText& guid, bool p_tryToLoadIfNotFound){\
        auto ptr = Load##restypename(guid,p_tryToLoadIfNotFound);\
        return restypename##ResPtr(guid,ptr);\
    }\
    void ResourcesUtils::Register##restypename(const StringText& guid, ##restypename* p_resPtr){\
        GetGlobalService<ResourceManagement::##restypename##Manager>().RegisterResource(guid,p_resPtr);\
    }

    // show macro for shader
    void ResourcesUtils::LoadRes(const std::string& guid, Shader*& ptr, bool p_tryToLoadIfNotFound )
   {
       ptr = LittleEngine::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().GetResource(
           guid, p_tryToLoadIfNotFound);
   }

    Shader* ResourcesUtils::LoadShader(const std::string& guid, bool p_tryToLoadIfNotFound)
    {
        return LittleEngine::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().GetResource(
            guid, p_tryToLoadIfNotFound);
    }

    ShaderResPtr ResourcesUtils::LoadShaderResPtr(const std::string& guid, bool p_tryToLoadIfNotFound)
    {
        auto ptr = LoadShader(guid, p_tryToLoadIfNotFound);
        return ShaderResPtr(guid, ptr);
    }

    void ResourcesUtils::RegisterShader(const std::string& guid, Shader* p_resPtr)
    {
        LittleEngine::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().RegisterResource(guid, p_resPtr);
    }

    //DEFINE_RES_LOAD_FUNCTION(Shader);
    DEFINE_RES_LOAD_FUNCTION(Texture);
    DEFINE_RES_LOAD_FUNCTION(Model);
    DEFINE_RES_LOAD_FUNCTION(Animation);
    DEFINE_RES_LOAD_FUNCTION(Sound);
    DEFINE_RES_LOAD_FUNCTION(Material);
}
