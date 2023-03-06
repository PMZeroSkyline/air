#version 330 core

layout(location = 0) in vec3 P;
layout(location = 1) in vec3 N;
layout(location = 2) in vec4 T;
layout(location = 3) in vec2 C;
layout(location = 4) in vec2 UV;
layout(location = 5) in vec2 UV1;

out V2F
{
	vec3 p;
	vec3 n;
	vec4 t;
	vec2 uv;
} o;

void main()
{
    gl_Position = vec4(P, 1);
	
	o.p = P;
	o.n = N;
	o.t = T;
	o.uv = UV;
}