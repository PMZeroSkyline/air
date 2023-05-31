#version 330 core
layout (location = 0) in vec3 POSITION;
layout (location = 7) in vec4 JOINTS_0;
layout (location = 8) in vec4 WEIGHTS_0;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform bool isSkin;
uniform mat4 J[128];


void main()
{
    mat4 S = mat4(1.f);
    if (isSkin)
    {
        S = WEIGHTS_0[0] * J[int(JOINTS_0[0])] + 
            WEIGHTS_0[1] * J[int(JOINTS_0[1])] + 
            WEIGHTS_0[2] * J[int(JOINTS_0[2])] + 
            WEIGHTS_0[3] * J[int(JOINTS_0[3])];
        S = M * S;
    }
    else
    {
        S = M;
    }

    gl_Position = P * V * S * vec4(POSITION, 1.f);

}