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
    vec4 baseColor = texture(baseColorTex, i.uv);
    if (baseColor.a < alphaCutoff)
        discard;
    FragColor = vec4(baseColor.rgb, 1.f);
}