#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D tB;
uniform sampler2D tN;
uniform sampler2D tP;

void main()
{   
    // FragColor = vec4(i.uv.x);
    FragColor = texture(tB, i.uv);
}