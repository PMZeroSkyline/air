#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec4 TANGENT;
layout (location = 3) in vec2 TEXCOORD_0;
layout (location = 4) in vec2 TEXCOORD_1;
layout (location = 5) in vec2 TEXCOORD_2;
layout (location = 6) in vec2 TEXCOORD_3;
layout (location = 7) in ivec4 JOINTS_0;
layout (location = 8) in vec4 WEIGHTS_0;

//layout (std140) uniform MTX
//{
//    mat4 V;
//    mat4 P;
//}; 

uniform bool isSkin;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 J[128];
out V2F
{
    vec2 uv;
} o;

void main()
{
    o.uv = TEXCOORD_0;

    vec4 pos = vec4(POSITION,1.0f);
    
    mat4 S = mat4(1.f);
    if (isSkin)
    {
        S = 
        WEIGHTS_0[0] * J[JOINTS_0[0]] + 
        WEIGHTS_0[1] * J[JOINTS_0[1]] + 
        WEIGHTS_0[2] * J[JOINTS_0[2]] + 
        WEIGHTS_0[3] * J[JOINTS_0[3]];
    }
    

    gl_Position = P * V * M * S * pos;

}