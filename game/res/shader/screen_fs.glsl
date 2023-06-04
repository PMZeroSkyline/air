#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D colorTex;

void main()
{   
    FragColor = texture(colorTex, i.uv);
}