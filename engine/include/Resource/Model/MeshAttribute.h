#ifndef MESH_ATTRIBUTE_H
#define MESH_ATTRIBUTE_H

#include "SDK/STL/STL.h"
#include "Core/Math/Math.h"

class MeshAttribute
{
public:
    vector<vec3> POSITION;
    vector<vec3> NORMAL;
    vector<vec4> TANGENT;
    vector<vec2> TEXCOORD_0;
    vector<vec2> TEXCOORD_1;
    vector<vec2> TEXCOORD_2;
    vector<vec2> TEXCOORD_3;
    vector<u8vec4> JOINTS_0;
    vector<vec4> WEIGHTS_0;
};

#endif
