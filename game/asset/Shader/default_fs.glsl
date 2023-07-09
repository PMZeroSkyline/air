#version 330 core
out vec4 FragColor;

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
    // vec3 worldPos = i.worldPos.xyz / i.worldPos.w;
    // vec3 worldNormal = normalize(i.worldNormal);
    // float nl = dot(worldNormal, normalize(vec3(1.f)));
    vec4 B = texture(tB, i.uv);
    if (bMask)
        if (B.a < .5f)
            discard;
    FragColor = B;
}