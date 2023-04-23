#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 debug;
} i;

uniform sampler2D baseColorTex;

void main()
{   
    FragColor = i.debug * 0.0001f;
    //FragColor = vec4(i.uv.x, i.uv.y, 0., 1.);
    // vec4 baseColor = texture(baseColorTex, i.uv);
    // if (baseColor.a < 0.5)
    //     discard;
    // FragColor = vec4(baseColor.rgb,1.);
}