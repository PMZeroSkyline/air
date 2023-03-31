#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
} i;

uniform sampler2D tex;
//uniform sampler2D baseColorTex;

void main()
{    
    //FragColor = vec4(i.uv.x, i.uv.y, 0., 1.);
    FragColor = texture(tex, i.uv);
    //FragColor = vec4(1.,0.,0.,1.);
}