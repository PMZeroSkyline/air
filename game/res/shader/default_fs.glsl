#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
} i;
uniform int alphaMode;
uniform sampler2D texB;
uniform vec3 viewPos;
uniform vec3 lightDir;

void main()
{   
    vec3 worldPos = i.worldPos.xyz / i.worldPos.w;
    vec3 worldNormal = normalize(i.worldNormal);
    vec3 viewDir = normalize(worldPos - viewPos);
    float nl = dot(worldNormal, normalize(lightDir));

    vec4 baseColor = texture(texB, i.uv);

    if (alphaMode == 1)
    {
        if (baseColor.a < .5f)
            discard;
    }
    FragColor = baseColor;
}