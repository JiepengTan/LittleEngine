#pragma once

namespace LittleEngine::Resources
{
    class Material;
}
namespace LittleEngine
{
    class RenderUtils
    {
    private:
        static unsigned int _QuadVAO;
        static unsigned int _QuadVBO;
        static std::vector<LittleEngine::Resources::Material*> _DebugMats;
        static LittleEngine::Resources::Material*  GetOrCreateDebugQuadMat();
    public:
        static void DrawDebugQuad(LittleEngine::Rendering::Resources::Texture* p_tex);
    };
}

