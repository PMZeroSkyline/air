#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D baseColorTex;

void main()
{   
    FragColor = vec4(i.uv.x, i.uv.y, 0., 1.);
    // vec4 baseColor = texture(baseColorTex, i.uv);
    // if (baseColor.a < 0.5)
    //     discard;
    // FragColor = vec4(baseColor.rgb,1.);
}