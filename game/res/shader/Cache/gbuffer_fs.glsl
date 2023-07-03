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
uniform sampler2D tB;
uniform bool bMask;
void main()
{   
    vec4 color = texture(tB, i.uv);
    if (bMask)
        if (color.a < .5f)
            discard;
    gPosition = i.worldPos;
    gNormal = normalize(i.worldNormal);
    gAlbedoSpec = color;
}
