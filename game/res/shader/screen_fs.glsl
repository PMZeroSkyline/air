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

#define SAMPLE_NUM 512
uniform mat4 lVP;
uniform vec3 lightDir;
uniform sampler2D tS;
uniform vec3 randPoints[SAMPLE_NUM];
uniform bool isUseZeroSkylineVolumeShadow;

float SoftShadow(vec3 position)
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

void main()
{   
    vec3 position = texture(tP, i.uv).xyz;
    vec3 color = texture(tC, i.uv).xyz;
    float shadow = SoftShadow(position);
    FragColor = vec4(color * mix(.1f, 1.f, shadow), 1.f);
    
}