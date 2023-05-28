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

    vec2 grid_src = i.uv;
    
    float s = 0.01f;
    vec2 f = fract(grid_src * 10.f);
    float lx_20cm = (f.x > s && f.x < (1.f-s)) ? 1.f : 0.f;
    float ly_20cm = (f.y > s && f.y < (1.f-s)) ? 1.f : 0.f;

    float grid_20cm;
    grid_20cm = lx_20cm * ly_20cm;
    float grid = mix(0.4f, 1.f, grid_20cm);
    grid = clamp(grid, 0.f, 1.f);
    float nl = dot(i.worldNormal, normalize(vec3(1.f)));
    nl = nl * .5f + .5f;
    vec3 col = grid == 1.f ? vec3(1.f) : vec3(grid);
    col *= nl;

    FragColor = vec4(col, 1.f);
}