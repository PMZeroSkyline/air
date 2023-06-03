#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D screenTex;

void main()
{   
    //FragColor = vec4(1.f, 0.f, 0.f, 1.f);
    FragColor = texture(screenTex, i.uv);
    
}