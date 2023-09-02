#shader vertex
#version 430 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Color;
layout (location = 3) in vec3 geo_Normal;
layout (location = 4) in vec3 geo_Tangent;
layout (location = 5) in vec3 geo_Bitangent;
layout (location = 6) in vec4 geo_BoneWeights;
layout (location = 7) in ivec4 geo_BoneIds; 

uniform mat4 u_LightSpaceMatrix;
uniform int u_IsSkinMesh;

/* Global information sent by the engine */
layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
    float   ubo_Time;
};

/* Information passed to the fragment shader */
out VS_OUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;
    mat3        TBN;
    flat vec3   TangentViewPos;
    vec3        TangentFragPos;
    vec4        FragPosLightSpace;
} vs_out;

void main()
{
    vs_out.TBN = mat3
    (
        normalize(vec3(ubo_Model * vec4(geo_Tangent,   0.0))),
        normalize(vec3(ubo_Model * vec4(geo_Bitangent, 0.0))),
        normalize(vec3(ubo_Model * vec4(geo_Normal,    0.0)))
    );

    mat3 TBNi = transpose(vs_out.TBN);

    vs_out.FragPos          = vec3(ubo_Model * vec4(geo_Pos, 1.0));
    vs_out.Normal           = normalize(mat3(transpose(inverse(ubo_Model))) * geo_Normal);
    vs_out.TexCoords        = geo_TexCoords;
    vs_out.TangentViewPos   = TBNi * ubo_ViewPos;
    vs_out.TangentFragPos   = TBNi * vs_out.FragPos;

    vs_out.FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

    gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
#version 430 core

/* Global information sent by the engine */
layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
    float   ubo_Time;
};

/* Information passed from the fragment shader */
in VS_OUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;
    mat3        TBN;
    flat vec3   TangentViewPos;
    vec3        TangentFragPos;    
    vec4        FragPosLightSpace;
} fs_in;

/* Light information sent by the engine */
layout(std430, binding = 0) buffer LightSSBO
{
    mat4 ssbo_Lights[];
};

// shadow map build in variable
uniform sampler2D   u_Shadowmap;
uniform vec4        u_ShadowLightPosition;
uniform mat4        u_LightSpaceMatrix;

/* Uniforms (Tweakable from the material editor) */
uniform vec2        u_TextureTiling           = vec2(1.0, 1.0);
uniform vec2        u_TextureOffset           = vec2(0.0, 0.0);
uniform vec4        u_Diffuse                 = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec3        u_Specular                = vec3(1.0, 1.0, 1.0);
uniform float       u_Shininess               = 100.0;
uniform float       u_HeightScale             = 0.0;
uniform bool        u_EnableNormalMapping     = false;
uniform sampler2D   u_DiffuseMap;
uniform sampler2D   u_SpecularMap;
uniform sampler2D   u_NormalMap;
uniform sampler2D   u_HeightMap;
uniform sampler2D   u_MaskMap;

/* Global variables */
vec3 g_Normal;
vec2 g_TexCoords;
vec3 g_ViewDir;
vec4 g_DiffuseTexel;
vec4 g_SpecularTexel;
vec4 g_HeightTexel;
vec4 g_NormalTexel;

out vec4 FRAGMENT_COLOR;

vec3 UnPack(float p_Target)
{
    return vec3
    (
        float((uint(p_Target) >> 24) & 0xff)    * 0.003921568627451,
        float((uint(p_Target) >> 16) & 0xff)    * 0.003921568627451,
        float((uint(p_Target) >> 8) & 0xff)     * 0.003921568627451
    );
}

bool PointInAABB(vec3 p_Point, vec3 p_AabbCenter, vec3 p_AabbHalfSize)
{
    return
    (
        p_Point.x > p_AabbCenter.x - p_AabbHalfSize.x && p_Point.x < p_AabbCenter.x + p_AabbHalfSize.x &&
        p_Point.y > p_AabbCenter.y - p_AabbHalfSize.y && p_Point.y < p_AabbCenter.y + p_AabbHalfSize.y &&
        p_Point.z > p_AabbCenter.z - p_AabbHalfSize.z && p_Point.z < p_AabbCenter.z + p_AabbHalfSize.z
    );
}

vec2 ParallaxMapping(vec3 p_ViewDir)
{
    const vec2 parallax = p_ViewDir.xy * u_HeightScale * texture(u_HeightMap, g_TexCoords).r;
    return g_TexCoords - vec2(parallax.x, 1.0 - parallax.y);
}

vec3 BlinnPhong(vec3 p_LightDir, vec3 p_LightColor, float p_Luminosity)
{
    const vec3  halfwayDir          = normalize(p_LightDir + g_ViewDir);
    const float diffuseCoefficient  = max(dot(g_Normal, p_LightDir), 0.0);
    const float specularCoefficient = pow(max(dot(g_Normal, halfwayDir), 0.0), u_Shininess * 2.0);

    return p_LightColor * g_DiffuseTexel.rgb * diffuseCoefficient * p_Luminosity + ((p_Luminosity > 0.0) ? (p_LightColor * g_SpecularTexel.rgb * specularCoefficient * p_Luminosity) : vec3(0.0));
}

float LuminosityFromAttenuation(mat4 p_Light)
{
    const vec3  lightPosition   = p_Light[0].rgb;
    const float constant        = p_Light[0][3];
    const float linear          = p_Light[1][3];
    const float quadratic       = p_Light[2][3];

    const float distanceToLight = length(lightPosition - fs_in.FragPos);
    const float attenuation     = (constant + linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

vec3 CalcPointLight(mat4 p_Light)
{
    /* Extract light information from light mat4 */
    const vec3 lightPosition  = p_Light[0].rgb;
    const vec3 lightColor     = UnPack(p_Light[2][0]);
    const float intensity     = p_Light[3][3];

    const vec3  lightDirection  = normalize(lightPosition - fs_in.FragPos);
    const float luminosity      = LuminosityFromAttenuation(p_Light);

    return BlinnPhong(lightDirection, lightColor, intensity * luminosity);
}

vec3 CalcDirectionalLight(mat4 light)
{
    return BlinnPhong(-light[1].rgb, UnPack(light[2][0]), light[3][3]);
}

vec3 CalcSpotLight(mat4 p_Light)
{
    /* Extract light information from light mat4 */
    const vec3  lightPosition   = p_Light[0].rgb;
    const vec3  lightForward    = p_Light[1].rgb;
    const vec3  lightColor      = UnPack(p_Light[2][0]);
    const float intensity       = p_Light[3][3];
    const float cutOff          = cos(radians(p_Light[3][1]));
    const float outerCutOff     = cos(radians(p_Light[3][1] + p_Light[3][2]));

    const vec3  lightDirection  = normalize(lightPosition - fs_in.FragPos);
    const float luminosity      = LuminosityFromAttenuation(p_Light);

    /* Calculate the spot intensity */
    const float theta           = dot(lightDirection, normalize(-lightForward)); 
    const float epsilon         = cutOff - outerCutOff;
    const float spotIntensity   = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    
    return BlinnPhong(lightDirection, lightColor, intensity * spotIntensity * luminosity);
}

vec3 CalcAmbientBoxLight(mat4 p_Light)
{
    const vec3  lightPosition   = p_Light[0].rgb;
    const vec3  lightColor      = UnPack(p_Light[2][0]);
    const float intensity       = p_Light[3][3];
    const vec3  size            = vec3(p_Light[0][3], p_Light[1][3], p_Light[2][3]);

    return PointInAABB(fs_in.FragPos, lightPosition, size) ? g_DiffuseTexel.rgb * lightColor * intensity : vec3(0.0);
}

vec3 CalcAmbientSphereLight(mat4 p_Light)
{
    const vec3  lightPosition   = p_Light[0].rgb;
    const vec3  lightColor      = UnPack(p_Light[2][0]);
    const float intensity       = p_Light[3][3];
    const float radius          = p_Light[0][3];

    return distance(lightPosition, fs_in.FragPos) <= radius ? g_DiffuseTexel.rgb * lightColor * intensity : vec3(0.0);
}


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_Shadowmap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(u_ShadowLightPosition.xyz - fs_in.FragPos);
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.002);
    // check whether current frag pos is in shadow
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    //return shadow;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_Shadowmap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_Shadowmap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    g_TexCoords = u_TextureOffset + vec2(mod(fs_in.TexCoords.x * u_TextureTiling.x, 1), mod(fs_in.TexCoords.y * u_TextureTiling.y, 1));

    /* Apply parallax mapping */
    if (u_HeightScale > 0)
        g_TexCoords = ParallaxMapping(normalize(fs_in.TangentViewPos - fs_in.TangentFragPos));


    /* Apply color mask */
    if (texture(u_MaskMap, g_TexCoords).r != 0.0)
    {
        g_ViewDir           = normalize(ubo_ViewPos - fs_in.FragPos);
        g_DiffuseTexel      = texture(u_DiffuseMap,  g_TexCoords) * u_Diffuse;
        g_SpecularTexel     = texture(u_SpecularMap, g_TexCoords) * vec4(u_Specular, 1.0);

        vec4 fragPosLightSpace = fs_in.FragPosLightSpace;
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        //FRAGMENT_COLOR = vec4(projCoords,1); return; 

        if (u_EnableNormalMapping)
        {
            g_Normal = texture(u_NormalMap, g_TexCoords).rgb;
            g_Normal = normalize(g_Normal * 2.0 - 1.0);   
            g_Normal = normalize(fs_in.TBN * g_Normal);
        }
        else
        {
            g_Normal = normalize(fs_in.Normal);
        }

        vec3 lightSum = vec3(0.0);
        vec3 ambient = vec3(0.0);
        lightSum += CalcDirectionalLight(ssbo_Lights[0]);
        ambient += CalcAmbientBoxLight(ssbo_Lights[1]);
        /*
        for (int i = 0; i < ssbo_Lights.length(); ++i)
        {
            switch(int(ssbo_Lights[i][3][0]))
            {
                case 0: lightSum += CalcPointLight(ssbo_Lights[i]);         break;
                case 1: lightSum += CalcDirectionalLight(ssbo_Lights[i]);   break;
                case 2: lightSum += CalcSpotLight(ssbo_Lights[i]);          break;
                case 3: ambient += CalcAmbientBoxLight(ssbo_Lights[i]);    break;
                case 4: lightSum += CalcAmbientSphereLight(ssbo_Lights[i]); break;
            }
        }   
*/ 
        //float shadow =texture(u_Shadowmap, fs_in.TexCoords.xy).r; 
        float shadow =1;// clamp( 1.0- ShadowCalculation(fs_in.FragPosLightSpace),0.2,1.0);    
           // perform perspective divide
        //FRAGMENT_COLOR = vec4(shadow); return;

        //FRAGMENT_COLOR = vec4(vec3((1.0 - shadow)),1); return;                   
        lightSum = ambient + shadow * lightSum; 
        FRAGMENT_COLOR = vec4(lightSum, g_DiffuseTexel.a);
    }
    else
    {
        FRAGMENT_COLOR = vec4(0.0);
    }
}