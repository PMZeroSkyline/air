#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D screenTex;

void main()
{   
    float c = texture(screenTex, i.uv).r;
    FragColor = vec4(vec3(c), 1.0);
    FragColor = vec4(0.f, i.uv.y, 0.f, 1.0);
}