#version 330 core
layout (location = 0) out vec4 tB;

in V2F
{
    vec2 uv;
} i;
uniform sampler2D tA;
const vec2 tSize = vec2(1.f / 800.f, 1.f / 600.f);
void main()
{   
    vec2 scale = tSize;
    vec4 a = texture(tA, i.uv + vec2( 1.f, 1.f) * scale);
    vec4 b = texture(tA, i.uv + vec2(-1.f, 1.f) * scale);
    vec4 c = texture(tA, i.uv + vec2(-1.f,-1.f) * scale);
    vec4 d = texture(tA, i.uv + vec2( 1.f,-1.f) * scale);
    tB = (a + b + c + d) * .25f;
}