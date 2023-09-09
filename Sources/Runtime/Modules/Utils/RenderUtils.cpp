#include "Core/Analytics/Profiling/ProfilerSpy.h"
#include "Modules/Framework/ECS/SceneRenderer.h"
#include "Modules/Framework/ECS/Components/CMaterialRenderer.h"
#include "Modules/Framework/Global/ServiceLocator.h"
#include "Resource/Manager/MaterialManager.h"
#include "Modules/Utils/RenderUtils.h"
#include "Resource/Loader/ShaderLoader.h"

unsigned int LittleEngine::RenderUtils::_QuadVAO = 0;
unsigned int LittleEngine::RenderUtils::_QuadVBO = 0;
std::vector<LittleEngine::Resources::Material*>LittleEngine::RenderUtils:: _DebugMats;

LittleEngine::Resources::Material* LittleEngine::RenderUtils::GetOrCreateDebugQuadMat()
{
    if(_DebugMats.size()>0) return _DebugMats[0];
    auto vertstr = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main()
{
    TexCoords = aTexCoords;
    vec3 pos= aPos;
    pos.xy *= 0.5;
    pos.xy += 0.5;
    gl_Position = vec4(pos, 1.0);
})";

    auto fragStr = R"(
#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D u_DebugTex;
uniform float near_plane;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float texVal = texture(u_DebugTex, TexCoords).r;
    // FragColor = vec4(vec3(LinearizeDepth(texVal) / far_plane), 1.0); // perspective
    FragColor = vec4(texVal,texVal,texVal, 1.0); // orthographic
    //FragColor = vec4(1,0,0,1);
})";

    auto shader	= LittleEngine::Rendering::Resources::Loaders::ShaderLoader::CreateFromSource(vertstr, fragStr);
    auto mat = new LittleEngine::Resources::Material();
    mat->SetDepthTest(false);
    mat->SetBlendable(false);
    mat->SetDepthWriting(false);
    
    mat->SetShader(shader);
    _DebugMats.push_back(mat);
    return _DebugMats[0];
}

void LittleEngine::RenderUtils::DrawDebugQuad(LittleEngine::Rendering::Resources::Texture* p_tex)
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
