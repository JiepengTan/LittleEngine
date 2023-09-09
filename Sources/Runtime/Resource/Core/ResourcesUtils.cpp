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
    restypename* ResourcesUtils::Load##restypename(StringText guid){\
        return GetGlobalService<ResourceManagement::##restypename##Manager>().GetResource(guid);\
    }\
    restypename##ResPtr ResourcesUtils::Load##restypename##ResPtr(StringText guid){\
        auto ptr = Load##restypename(guid);\
        return restypename##ResPtr(guid,ptr);\
    }
    
    DEFINE_RES_LOAD_FUNCTION(Shader);
    DEFINE_RES_LOAD_FUNCTION(Texture);
    DEFINE_RES_LOAD_FUNCTION(Model);
    DEFINE_RES_LOAD_FUNCTION(Animation);
    DEFINE_RES_LOAD_FUNCTION(Sound);
    DEFINE_RES_LOAD_FUNCTION(Material);
}
