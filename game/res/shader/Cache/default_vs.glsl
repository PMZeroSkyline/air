#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 1) in vec3 NORMAL;
layout (location = 2) in vec2 TEXCOORD_0;
layout (location = 3) in vec4 JOINTS_0;
layout (location = 4) in vec4 WEIGHTS_0;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform bool isSkin;
uniform mat4 J[128];
out V2F
{
    vec2 uv;
    vec4 worldPos;
    vec3 worldNormal;
} o;
void main()
{
    mat4 Model;
    if (isSkin)
    {
        mat4 S = 
            WEIGHTS_0[0] * J[int(JOINTS_0[0])] + 
            WEIGHTS_0[1] * J[int(JOINTS_0[1])] + 
            WEIGHTS_0[2] * J[int(JOINTS_0[2])] + 
            WEIGHTS_0[3] * J[int(JOINTS_0[3])];
        Model = M * S;
    }
    else
    {
        Model = M;
    }
    vec4 worldPos = Model * vec4(POSITION, 1.f);
    o.uv = TEXCOORD_0;
    o.worldPos = worldPos;
    o.worldNormal = (Model * vec4(NORMAL, 0.f)).xyz;
    gl_Position = P * V * worldPos;
}
