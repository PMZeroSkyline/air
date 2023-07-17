#version 330 core
out vec4 FragColor;
in V2F
{
    vec2 uv;
} i;
// Gbuffer
uniform sampler2D tC;
uniform sampler2D tN;
uniform sampler2D tP;
uniform sampler2D tD;
// SSSM
#define SAMPLE_NUM 64
uniform mat4 lVP;
uniform vec3 lightDir;
uniform sampler2D tS;
uniform vec3 randPoints[SAMPLE_NUM];
// 
uniform mat4 VP;
uniform mat4 P;
uniform sampler2D tNoise;
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);

float ScreenSpaceDirectionOcclusion(vec3 position)
{
    float light = 0;
    for (int i = 0; i != SAMPLE_NUM; i++)
    {
        vec3 v = randPoints[i];
        v *= pow(length(v), 5.f) * 5.f;
        vec4 lightFragPos = lVP * vec4(position + v, 1.f);
        vec3 lightNDC01 = lightFragPos.xyz / lightFragPos.w * .5f + .5f;
        float lightFragZ = texture(tS, lightNDC01.xy).r; 
        light += lightNDC01.z < lightFragZ  ? 1.f : 0.f;        
    }
    light /= SAMPLE_NUM;
    light = clamp(light * 2.f, 0.f, 1.f);
    return light;
}
vec3 ScreenSpaceGlobalIllumination(vec3 position, vec3 normal)
{
    vec3 light = vec3(0.f);

    for (int i = 0; i != SAMPLE_NUM; i++)
    {
        vec3 v = randPoints[i];
        // v *= pow(length(v), 2.f);
        vec4 fragPos = VP * vec4(position + v, 1.f);
        vec3 ndc01 = fragPos.xyz / fragPos.w * .5f + .5f;
        vec3 scatterColor = texture(tC, ndc01.xy).rgb; 
        vec3 scatterNormal = texture(tN, ndc01.xy).rgb; 
        vec3 scatterPos = texture(tP, ndc01.xy).rgb; 
        float ndn = dot(normal, scatterNormal);
        light += scatterColor * vec3(clamp(1.f-ndn, 0.f, 1.f));
        // light += vec3(distance(scatterPos, position) * 0.1f);
    }
    light /= SAMPLE_NUM;
    light = clamp(light * 2.f, 0.f, 1.f);
    return light;
}
vec3 normal_from_depth(float depth, vec2 texcoords) {
  
  const vec2 offset1 = vec2(0.0,0.001);
  const vec2 offset2 = vec2(0.001,0.0);
  
  float depth1 = texture(tD, texcoords + offset1).r;
  float depth2 = texture(tD, texcoords + offset2).r;
  
  vec3 p1 = vec3(offset1, depth1 - depth);
  vec3 p2 = vec3(offset2, depth2 - depth);
  
  vec3 normal = cross(p1, p2);
  normal.z = -normal.z;
  
  return normalize(normal);
}

void main()
{   
    vec3 col = texture(tC, i.uv).xyz;
    FragColor = vec4(col, 1.f);
    
}