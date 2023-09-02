#shader vertex
#version 430 core
layout (location = 0) in vec3 geo_Pos;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Local2World;
out vec4 FragPos;

out VS_OUT
{
    vec4 FragPos;
} vs_out;

void main()
{
    vs_out.FragPos = u_LightSpaceMatrix * u_Local2World * vec4(geo_Pos, 1.0);
    gl_Position = vs_out.FragPos;
}


#shader fragment
#version 430 core
out vec4 FRAGMENT_COLOR;
in VS_OUT
{
    vec4 FragPos;
} fs_in;
void main()
{            
    float zVal = fs_in.FragPos.z/fs_in.FragPos.w;
    zVal = zVal *0.5+0.5;
    FRAGMENT_COLOR = vec4(zVal,zVal,zVal,1);
}
