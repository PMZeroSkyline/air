#version 330 core
out vec4 FragColor;
in V2F
{
    vec2 uv;
} i;
uniform sampler2D tB;
uniform sampler2D tN;
uniform sampler2D tP;
uniform mat4 V;
uniform mat4 P;
uniform vec3 viewPos;
uniform vec3 rands[64];
const vec2 scale = vec2(1.0f / 800.f, 1.0f / 600.f);
void main()
{   
    vec3 col = texture(tB, i.uv).xyz;
    vec3 pos = texture(tP, i.uv).xyz;
    vec4 viewPos = V * vec4(pos, 1.f);
    FragColor = vec4(col, 1.f);
}
