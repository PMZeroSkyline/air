#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec2 TEXCOORD_0;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
out V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
} o;
void main()
{
    o.uv = TEXCOORD_0;
    vec4 wp =  M * vec4(POSITION, 1.f);
    o.worldPos = wp;
    o.worldNormal = (M * vec4(NORMAL, 0.f)).xyz;
    gl_Position = P * V * wp;
}
