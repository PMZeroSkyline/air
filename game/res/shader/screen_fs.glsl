#version 330 core
out vec4 FragColor;
in V2F
{
    vec2 uv;
} i;
uniform mat4 lVP;
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform sampler2D tC;
uniform sampler2D tN;
uniform sampler2D tP;
uniform sampler2D tS;
uniform sampler2D tD;
#define SSSM_SAMPLE 64
uniform vec3 randPoints[SSSM_SAMPLE];
float GetZeroSkylineVolumeShadow(vec3 position)
{
    float shadow = 0;
    for (int i = 0; i != SSSM_SAMPLE; i++)
    {
        vec3 v = randPoints[i];
        v *= pow(length(v), 2.f);
        vec4 lightFragPos = lVP * vec4(position + v, 1.f);
        vec3 lightNDC01 = lightFragPos.xyz / lightFragPos.w * .5f + .5f;
        float lightFragZ = texture(tS, lightNDC01.xy).r; 
        shadow += lightNDC01.z > lightFragZ  ? 1.0 : 0.0;        
    }
    shadow = 1.f - shadow / SSSM_SAMPLE;
    return shadow;
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
uniform bool isUseZeroSkylineVolumeShadow;
void main()
{   
    vec3 color = texture(tC, i.uv).xyz;
    vec3 position = texture(tP, i.uv).xyz;
    vec3 normal = texture(tN, i.uv).xyz;
    float shadow = 0.f;
    if (isUseZeroSkylineVolumeShadow)
    {
        shadow = GetZeroSkylineVolumeShadow(position);
    }
    else
    {
        shadow = GetShadow(position);
    }
    FragColor = vec4(color * mix(.2f, 1.f, shadow), 1.f);
}