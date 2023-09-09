#pragma once

namespace LittleEngine
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
        static std::vector<LittleEngine::Material*> _DebugMats;
        static LittleEngine::Material*  GetOrCreateDebugQuadMat();
    public:
        static void DrawDebugQuad(LittleEngine::Texture* p_tex);
    };
}

