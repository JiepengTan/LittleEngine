#include "ResourcesUtils.h"

#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/AnimationManager.h"
#include "Resource/Manager/MaterialManager.h"
#include "Resource/Manager/ModelManager.h"
#include "Resource/Manager/ShaderManager.h"
#include "Resource/Manager/SoundManager.h"
#include "Resource/Manager/TextureManager.h"

namespace LittleEngine
{
    
#define DEFINE_LOADFUNCTION(restypename)\
    restypename* ResourcesUtils::Load##restypename(StringText guid){\
        return GetGlobalService<ResourceManagement::##restypename##Manager>().GetResource(guid);\
    }
    
    DEFINE_LOADFUNCTION(Shader);
    DEFINE_LOADFUNCTION(Texture);
    DEFINE_LOADFUNCTION(Model);
    DEFINE_LOADFUNCTION(Animation);
    DEFINE_LOADFUNCTION(Sound);
    DEFINE_LOADFUNCTION(Material);
}
