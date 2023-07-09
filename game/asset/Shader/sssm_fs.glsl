#version 330 core
layout (location = 0) out vec4 tB;
in V2F
{
    vec2 uv;
} i;
uniform mat4 lVP;
uniform vec3 lightDir;
uniform sampler2D tP;
uniform sampler2D tS;
#define SAMPLE_NUM 64
uniform vec3 randPoints[SAMPLE_NUM];
uniform bool isUseZeroSkylineVolumeShadow;

float GetZeroSkylineVolumeShadow(vec3 position)
{
    float light = 0;
    for (int i = 0; i != SAMPLE_NUM; i++)
    {
        vec3 v = randPoints[i];
        v *= pow(length(v), 2.f);
        vec4 lightFragPos = lVP * vec4(position + v, 1.f);
        vec3 lightNDC01 = lightFragPos.xyz / lightFragPos.w * .5f + .5f;
        float lightFragZ = texture(tS, lightNDC01.xy).r; 
        light += lightNDC01.z < lightFragZ  ? 1.f : 0.f;        
    }
    light /= SAMPLE_NUM;
    return light;
}
float GetShadow(vec3 position)
{
    float shadow = 0;
    vec4 lightFragPos = lVP * vec4(position, 1.f);
    vec3 lightNDC01 = lightFragPos.xyz / lightFragPos.w * .5f + .5f;
    float lightFragZ = texture(tS, lightNDC01.xy).r; 
    shadow += lightNDC01.z > lightFragZ  ? 0.0 : 1.0;        
    return shadow;
}
void main()
{   
    vec3 position = texture(tP, i.uv).xyz;
    float shadow = 0.f;
    if (isUseZeroSkylineVolumeShadow)
    {
        shadow = GetZeroSkylineVolumeShadow(position);
    }
    else
    {
        shadow = GetShadow(position);
    }
    tB = vec4(shadow);
}