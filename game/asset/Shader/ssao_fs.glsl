#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D tB;
uniform sampler2D tN;
uniform sampler2D tP;

// uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 viewPos;
uniform mat4 lV;
uniform mat4 lP;
uniform vec3 lightDir;

// uniform bool isSkin;
// uniform bool isMask;
void main()
{   
    vec3 col = texture(tB, i.uv).xyz;
    vec3 pos = texture(tP, i.uv).xyz;
    vec3 normal = texture(tN, i.uv).xyz;

    float nl = dot(normal, lightDir);
    FragColor = vec4(1.f);
    
}