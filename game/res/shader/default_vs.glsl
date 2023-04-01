#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec4 TANGENT;
layout (location = 3) in vec2 TEXCOORD_0;
layout (location = 4) in vec2 TEXCOORD_1;
layout (location = 5) in vec2 TEXCOORD_2;
layout (location = 6) in vec2 TEXCOORD_3;
layout (location = 7) in uvec3 JOINTS_0;
layout (location = 8) in vec3 WEIGHTS_0;

// layout (std140) uniform Matrices
// {
//     mat4 VIEW;
//     mat4 PROJECTION;
// }; 
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out V2F
{
    vec2 uv;
} o;

void main()
{
    o.uv = TEXCOORD_0;
    gl_Position = P * V * M * vec4(POSITION, 1.0);
}