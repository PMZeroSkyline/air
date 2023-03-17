#version 330 core
out vec4 FragColor;
in V2F
{
	vec3 p;
	vec3 n;
	vec4 t;
	vec2 uv;
} i;
uniform float iTime;
vec2 iResolution = vec2(800, 600);
struct ray 
{
    vec3 p;
    float t;
    vec3 d;
};
struct hit
{
    float t;
    vec3 p;
    vec3 n;
};
float sd_cube(vec3 p, vec3 c, vec3 e) {
    vec3 d = abs(p - c) - e;
    float i = min(max(d.x, max(d.y, d.z)), 0.);
    float o = length(max(d, 0.));
    return i + o;
}
float sd_sphere(vec3 p, vec3 c, float r) 
{
    return length(p-c)-r; 
}
float sdf(vec3 p)
{
    float d0 = sd_cube(p, vec3(0.), vec3(1.));
    float r = abs(sin(iTime));
    float d1 = sd_sphere(p, vec3(1,0.,0.), r);
    float d2 = sd_sphere(p, vec3(-1,0.,0.), r);
    float d3 = sd_sphere(p, vec3(0.,0.,1), r);
    float d4 = sd_sphere(p, vec3(0.,0.,-1), r);
    float d5 = sd_sphere(p, vec3(0.), r*4.);
    return max(max(d0, max(-d1, max(-d2, max(-d3, -d4)))),d5);
}
vec3 pixel_dir(float fov, vec2 fragCoord) 
{
    vec3 dir;
    dir.xy = fragCoord-iResolution.xy*.5;
    dir.z = -iResolution.y*.5/tan(radians(fov)*.5);
    return normalize(dir);
}
vec3 get_normal(vec3 p)
{
    float d = .001;
    float x = sdf(vec3(p.x+d,p.y,p.z)) - sdf(vec3(p.x-d, p.y, p.z));
    float y = sdf(vec3(p.x,p.y+d,p.z)) - sdf(vec3(p.x, p.y-d, p.z));
    float z = sdf(vec3(p.x,p.y,p.z+d)) - sdf(vec3(p.x, p.y, p.z-d));
    return normalize(vec3(x,y,z));
}
hit marching(ray r) 
{
    hit h;
    float t = r.t;
    for(int i = 0; i != 256; i++) 
    { 
        vec3 p = r.p + vec3(t) * r.d;
        float sd = sdf(p);
        if(sd < .0001 || t > 100.f) 
        {
            h.n = get_normal(p); 
            h.t = t;
            h.p = p;
            return h;
        } 
        t += sd; 
    }
    return h; 
} 
float blinn_phong(vec3 l, vec3 v, vec3 n, vec3 p)
{
    vec3 h = normalize(l + v);
    return dot(n, h);
}
mat4 viewMtx(vec3 p, vec3 lookAt, vec3 up)
{
    vec3 f = normalize(lookAt-p);
    vec3 r = normalize(cross(f,up));
    vec3 u = cross(r, f);
    mat4 m = mat4(
                 vec4(r,0.),
                 vec4(u,0.),
                 vec4(-f,0.),
                 vec4(0.,0.,0.,1.));
    return m;
}
void mainImage( out vec4 fragColor, in vec2 fragCoord ) 
{
    float rotY;
	rotY=iTime;
    vec3 cameraPos = vec3(sin(rotY)*4.,sin(iTime)*2.,cos(rotY)*4.);
    
    mat4 V = viewMtx(cameraPos, vec3(0.), vec3(0, 1.,0));
    
    vec3 viewDir = pixel_dir(45., fragCoord);;
    vec3 worldDir = (V * vec4(viewDir, 0.)).xyz;
    vec3 dirLightDir = normalize(vec3(1.));

    ray r; 
    r.p = cameraPos;
    r.d = worldDir;
    
    hit res = marching(r);
    
    if(res.t < 100.)
    {
        float nl = dot(res.n,dirLightDir);
        vec3 v = normalize(cameraPos-res.p);
        vec3 h = normalize(v + dirLightDir);
        
        vec3 dirLightCol = vec3(3.14);
        vec3 envCol = vec3(.1,.2,.4);
        
        float lambert = max(nl,0.);
        
        vec3 indirL = envCol;
        vec3 dirL = dirLightCol * lambert;
        vec3 diff = (dirL + indirL) / 3.14;
        
        float blinn_phong = pow(max(dot(h, res.n),0.),30.)*max(nl,0.) / 3.14;
        float F = .04 + pow(1.-dot(res.n, v), 5.);
        vec3 spec = dirLightCol * blinn_phong + envCol * F;
        
        vec3 col = spec + diff;
        
        col = pow(col, vec3(.45));
        
        fragColor = vec4(col,1.);
    }
    else
    {
        fragColor = vec4(.1,.2,.4,1.);
    }
}
void main()
{
	vec4 fragColor;
	vec2 fragCoord = iResolution * i.uv;
	mainImage(fragColor, fragCoord);
	FragColor = fragColor;
}