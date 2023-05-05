#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Core/Math/Math.h"

class BoundingBox
{
public:
    vec3 min = vec3(0.f);
    vec3 max = vec3(0.f);
    bool Check(const vec3& v)
    {
        bool x = v.x < max.x && v.x > min.x;
        bool y = v.y < max.y && v.y > min.y;
        //bool z = v.z < max.z && v.z > min.z;
        return x && y;
    }
};

#endif