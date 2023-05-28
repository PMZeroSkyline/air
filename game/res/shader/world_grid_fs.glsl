#version 330 core
out vec4 FragColor;

in V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
    vec3 viewPos;
} i;
void main()
{   
    float a = sin(radians(45.f));

    vec3 grid_src = i.worldPos.xyz;
    
    vec3 f = fract(grid_src);
    float s = 0.01f;
    float lx_m = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_m = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;
    float lz_m = (f.z > s && f.z < (1.f-s)) ? 1.f : 0.f;

    s = 0.01f;
    f = fract(grid_src * 5.f);
    float lx_20cm = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_20cm = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;
    float lz_20cm = (f.z > s && f.z < (1.f-s)) ? 1.f : 0.f;

    s = 0.0015f;
    f = fract(grid_src * .1f);
    float lx_10m = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_10m = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;
    float lz_10m = (f.z > s && f.z < (1.f-s)) ? 1.f : 0.f;


    bool mask_z = abs(dot(vec3(0.f, 0.f, 1.f), i.worldNormal)) > a;
    bool mask_x = abs(dot(vec3(1.f, 0.f, 0.f), i.worldNormal)) > a;
    bool mask_y = abs(dot(vec3(0.f, 1.f, 0.f), i.worldNormal)) > a;
    
    float grid_m;
    float grid_10m;
    float grid_20cm;
    if (mask_z)
    {
        grid_m = lx_m * ly_m;
        grid_20cm = lx_20cm * ly_20cm;
        grid_10m = lx_10m * ly_10m;
    }
    else if(mask_y)
    {
        grid_m = lx_m * lz_m;
        grid_20cm = lx_20cm * lz_20cm;
        grid_10m = lx_10m * lz_10m;
    }
    else if(mask_x)
    {
        grid_m = lz_m * ly_m;
        grid_20cm = lz_20cm * ly_20cm;
        grid_10m = lz_10m * ly_10m;
    }
    else
    {
        grid_m = lx_m * ly_m * lz_m;
        grid_20cm = lx_20cm * ly_20cm * lz_20cm;
        grid_10m = lx_10m * ly_10m * lz_10m;
    }
    grid_20cm = 1.f - (1.f - distance(i.viewPos, i.worldPos.xyz) * 0.1f) * (1.f - grid_20cm);
    grid_m = 1.f - (1.f - distance(i.viewPos, i.worldPos.xyz) * 0.05f) * (1.f - grid_m);
    grid_10m = 1.f - (1.f - distance(i.viewPos, i.worldPos.xyz) * 0.01f) * (1.f - grid_10m);
    float grid = mix(0.2f, 1.f, grid_10m) * mix(.3f, 1.f, grid_m) * mix(0.4f, 1.f, grid_20cm);
    grid = clamp(grid, 0.f, 1.f);
    float nl = dot(i.worldNormal, normalize(vec3(1.f)));
    nl = nl * .5f + .5f;
    vec3 col = grid == 1.f ? vec3(1.f) : vec3(grid);
    col *= nl;

    FragColor = vec4(col, 1.f);
}