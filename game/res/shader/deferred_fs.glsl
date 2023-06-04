#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D gBaseColorRoughness;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDepth;
uniform sampler2D lDepth;

uniform vec3 viewPos;
uniform vec3 lightDir;
uniform mat4 cameraIVP;
uniform mat4 lightVP;



void main()
{   
    vec2 uv = i.uv;
    vec4 baseColorRoughness = texture(gBaseColorRoughness, uv);
    vec3 worldPos = texture(gPosition, uv).rgb;
    vec3 worldNormal = texture(gNormal, uv).rgb;
    float depth = texture(gDepth, uv).r;

    //vec3 col = baseColorRoughness.rgb;

    vec3 col;
    if (i.uv.x < .25f)
        col = baseColorRoughness.rgb;
    if (i.uv.x > .25f && i.uv.x < .5f)
        col = worldPos;
    if (i.uv.x > .5f && i.uv.x < .75f)
        col = worldNormal;
    if (i.uv.x > .75f)
        col = vec3(depth);

    // Screen Space Shadow
    vec4 camClipPos = vec4(vec3(uv.x, uv.y, depth) * 2.f - 1.f, 1.f);
    vec4 camWorldPos = cameraIVP * camClipPos;
    camWorldPos = camWorldPos / camWorldPos.w;
    vec4 camLightPos = lightVP * camWorldPos;
    vec2 camLightClipUV = camLightPos.xy * .5f + .5f;
    camLightPos = camLightPos / camLightPos.w;
    float camLightDpeth = camLightPos.z * .5f + .5f;
    float lightDepth = texture(lDepth, camLightClipUV).r;
    float bias = max(.005f * (1 - dot(worldNormal, lightDir)), .005f);
    float shadow = (camLightDpeth - bias) < lightDepth ? 1.f : 0.f;

    float nl = dot(lightDir, worldNormal);
    nl = max(nl, 0.f);

    col = baseColorRoughness.rgb * shadow * nl;
    col += baseColorRoughness.rgb * vec3(.5f);

    FragColor = vec4(col, 1.f);
    //FragColor = vec4(1.f - dot(worldNormal, lightDir));
}