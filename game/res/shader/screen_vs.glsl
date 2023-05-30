#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 3) in vec2 TEXCOORD_0;

out V2F
{
    vec2 uv;
} o;

void main()
{
    o.uv = TEXCOORD_0;
    gl_Position = vec4(POSITION.x, POSITION.y, 0.f, 1.f);
}