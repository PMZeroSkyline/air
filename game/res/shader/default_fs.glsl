#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 worldPos;
} i;

uniform sampler2D baseColorTex;
uniform bool isUseTexture;

void main()
{   
    //FragColor = vec4(i.worldPos);
    // FragColor = vec4(i.uv.x, i.uv.y, 0., 1.);
    vec4 baseColor = vec4(.5f);
    if(isUseTexture)
    {
        baseColor = texture(baseColorTex, i.uv);
        if (baseColor.a < 0.5)
            discard;
    }
    FragColor = baseColor;
}