#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
    vec3 viewPos;
} i;
void main()
{   
    FragColor = vec4(i.worldNormal, 1.f);
}