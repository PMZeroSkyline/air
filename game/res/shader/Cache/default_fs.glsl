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
    vec4 B = texture(tB, i.uv);
    if (bMask)
        if (B.a < .5f)
            discard;
    FragColor = B;
}
