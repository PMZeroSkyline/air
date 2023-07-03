#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D tNoise;
uniform sampler2D tN;
uniform sampler2D tP;

uniform float time;
uniform mat4 P;
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);

void main()
{   
    vec3 pos = texture(tP, i.uv).xyz;
    vec3 normal = texture(tN, i.uv).xyz;
    // vec3 randomVec = texture(tNoise, i.uv * noiseScale).xzy;
    // vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 tangent = vec3(1.f, 0.f, 0.f);
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    vec3 col = TBN * pos;
    FragColor = vec4(col, 1.f);
    // FragColor = texture(tN, i.uv);
}