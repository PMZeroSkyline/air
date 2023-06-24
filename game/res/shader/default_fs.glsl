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

uniform samplerCube texCube;

void main()
{   
    vec3 worldPos = i.worldPos.xyz / i.worldPos.w;
    vec3 worldNormal = normalize(i.worldNormal);
    vec3 viewDir = normalize(worldPos - viewPos);
    float nl = dot(worldNormal, normalize(lightDir));

    float s = sin(radians(90.f));
    float c = cos(radians(90.f));
    mat3 R = mat3(
        1.0, 0.0, 0.0,
        0.0, c, -s,
        0.0, s, c
    );
    vec3 coord = R * viewDir;
    //vec4 baseColor = texture(texB, i.uv);
    vec4 baseColor = texture(texCube, normalize(coord));

    if (alphaMode == 1)
    {
        if (baseColor.a < .5f)
            discard;
    }
    baseColor.rgb = pow(baseColor.rgb, vec3(.45f));
    FragColor = baseColor;
}