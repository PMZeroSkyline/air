#version 330 core
out vec4 FragColor;


in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
} i;

uniform bool isUseTex;
uniform sampler2D colorTex;
uniform vec3 viewPos;
uniform vec3 lightDir;




void main()
{   
    vec3 worldPos = i.worldPos.xyz / i.worldPos.w;
    vec3 worldNormal = normalize(i.worldNormal);
    float nl = dot(worldNormal, normalize(lightDir));

    //float grid = WorldGrid(worldPos, worldNormal);

    vec3 col = vec3(1.f) * nl * .5f + .5f;
    float alpha = 1.f;

    if(isUseTex)
    {
        vec4 colTex = texture(colorTex, i.uv);
        col = colTex.xyz;
        col = dot(worldNormal, normalize(lightDir)) > 0.f ? col : col * col;
        alpha = colTex.a;
        if (alpha < 0.5f)
            discard;
    }

    #ifdef TEST
    col *= vec3(1.f, 0.f, 0.f);
    #endif
    
    FragColor = vec4(col, alpha);
}