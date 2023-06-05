#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
} i;

uniform bool isUseTex;
uniform sampler2D baseColorTex;
uniform bool isBlend;
uniform vec3 viewPos;
uniform vec3 lightDir;

float WorldGrid(vec3 worldPos, vec3 worldNormal)
{
    float a = sin(radians(45.f));

    vec3 grid_src = worldPos;
    
    vec3 f = fract(grid_src);
    float s = 0.01f;
    float lx_m = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_m = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;
    float lz_m = (f.z > s && f.z < (1.f-s)) ? 1.f : 0.f;

    s = 0.01f;
    f = fract(grid_src * 2.f);
    float lx_50cm = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_50cm = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;
    float lz_50cm = (f.z > s && f.z < (1.f-s)) ? 1.f : 0.f;

    s = 0.0015f;
    f = fract(grid_src * .1f);
    float lx_10m = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_10m = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;
    float lz_10m = (f.z > s && f.z < (1.f-s)) ? 1.f : 0.f;


    bool mask_z = abs(dot(vec3(0.f, 0.f, 1.f), worldNormal)) > a;
    bool mask_x = abs(dot(vec3(1.f, 0.f, 0.f), worldNormal)) > a;
    bool mask_y = abs(dot(vec3(0.f, 1.f, 0.f), worldNormal)) > a;
    
    float grid_m;
    float grid_10m;
    float grid_50cm;
    if (mask_z)
    {
        grid_m = lx_m * ly_m;
        grid_50cm = lx_50cm * ly_50cm;
        grid_10m = lx_10m * ly_10m;
    }
    else if(mask_y)
    {
        grid_m = lx_m * lz_m;
        grid_50cm = lx_50cm * lz_50cm;
        grid_10m = lx_10m * lz_10m;
    }
    else if(mask_x)
    {
        grid_m = lz_m * ly_m;
        grid_50cm = lz_50cm * ly_50cm;
        grid_10m = lz_10m * ly_10m;
    }
    else
    {
        grid_m = lx_m * ly_m * lz_m;
        grid_50cm = lx_50cm * ly_50cm * lz_50cm;
        grid_10m = lx_10m * ly_10m * lz_10m;
    }
    float disMask = max(1.f - distance(viewPos, worldPos) * 0.05f, 0.f);
    float grid = min(grid_m, min(grid_50cm, grid_10m));
    return (1.f - (1.f - grid) * disMask);
}

float UVGrid(vec2 uv)
{
    float s = 0.01f;

    vec2 f = fract(uv * 10.f);
    float lx_20cm = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_20cm = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;

    return lx_20cm * ly_20cm;
}

void main()
{   
    vec3 worldPos = i.worldPos.xyz / i.worldPos.w;
    vec3 worldNormal = normalize(i.worldNormal);
    float nl = dot(worldNormal, normalize(vec3(1.f))) * .5f + .5f;

    float grid = WorldGrid(worldPos, worldNormal);

    vec3 col = vec3(1.f) * grid * nl;

    if(isUseTex)
    {
        col *= texture(baseColorTex, i.uv).xyz;
    }
    float a = isBlend ? .5f : 1.f;
    FragColor = vec4(col, a);
}