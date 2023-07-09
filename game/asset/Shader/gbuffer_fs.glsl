#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
} i;

uniform sampler2D tC;
uniform bool isMask;

uniform mat4 V;
uniform mat4 P;

void main()
{   
    // vec3 worldPos = i.worldPos.xyz / i.worldPos.w;
    // vec3 worldNormal = normalize(i.worldNormal);
    // float nl = dot(worldNormal, normalize(vec3(1.f)));

    vec4 color = texture(tC, i.uv);
    if (isMask)
        if (color.a < .5f)
            discard;

    gPosition = i.worldPos;
    gNormal = normalize(i.worldNormal);
    gAlbedoSpec = color;
}