#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
    vec3 viewPos;
} i;

uniform sampler2D baseColorTex;
uniform float alphaCutoff;

void main()
{   
    FragColor = vec4(1.f, 0.f, 1.f, 1.f);
}