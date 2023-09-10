#include "Core/Analytics/Profiling/ProfilerSpy.h"
#include "Modules/Framework/ECS/SceneRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/MaterialManager.h"
#include "Modules/Utils/RenderUtils.h"
#include "Resource/Loader/ShaderLoader.h"

unsigned int LittleEngine::RenderUtils::_QuadVAO = 0;
unsigned int LittleEngine::RenderUtils::_QuadVBO = 0;
std::vector<LittleEngine::Material*>LittleEngine::RenderUtils:: _DebugMats;

LittleEngine::Material* LittleEngine::RenderUtils::GetOrCreateDebugQuadMat()
{
    if(_DebugMats.size()>0) return _DebugMats[0];
    auto shader	= ResourcesUtils::LoadShader(":Shaders\\DebugQuad.glsl"); 
    auto mat = new LittleEngine::Material();
    mat->SetDepthTest(false);
    mat->SetBlendable(false);
    mat->SetDepthWriting(false);
    
    mat->SetShader(shader);
    _DebugMats.push_back(mat);
    return _DebugMats[0];
}

void LittleEngine::RenderUtils::DrawDebugQuad(LittleEngine::Texture* p_tex)
{
    auto material = GetOrCreateDebugQuadMat();
    if (material == nullptr) return;
    material->Set("u_DebugTex", p_tex);
    material->Set("near_plane", 0.1f);
    material->Set("far_plane", 100);
    material->Bind(nullptr);
    if (_QuadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &_QuadVAO);
        glGenBuffers(1, &_QuadVBO);
        glBindVertexArray(_QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(_QuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    material->UnBind();
}
