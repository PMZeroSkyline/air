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
float ShadowCalculation(vec4 fragPosLightSpace, float currentDepth)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(tS, projCoords.xy).r; 
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}
void main()
{   
    vec3 color = texture(tC, i.uv).xyz;
    vec3 position = texture(tP, i.uv).xyz;
    vec3 normal = texture(tN, i.uv).xyz;
    float depth = texture(tD, i.uv).x;
    vec3 shadowSpacePos = lVP * vec4(position, 1.f);
    float shadow = ShadowCalculation(shadowSpacePos.xyz, depth);
    FragColor = vec4(shadow);
}
