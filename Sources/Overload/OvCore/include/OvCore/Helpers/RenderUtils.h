#pragma once

namespace OvCore::Resources
{
    class Material;
}
namespace OvCore::ECS
{
    class RenderUtils
    {
    private:
        static unsigned int _QuadVAO;
        static unsigned int _QuadVBO;
        static std::vector<OvCore::Resources::Material*> _DebugMats;
        static OvCore::Resources::Material*  GetOrCreateDebugQuadMat();
    public:
        static void DrawDebugQuad(OvRendering::Resources::Texture* p_tex);
    };
}

