#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 3) in vec2 TEXCOORD_0;

out V2F
{
    vec2 uv;
} o;

uniform vec4 ST;

void main()
{
    o.uv = TEXCOORD_0;
    vec2 pos = POSITION.xy * ST.xy + ST.zw;
    gl_Position = vec4(pos.x, pos.y, 0.f, 1.f);
}