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
        for (int i = 0; i < 3; i++)
        {
            if (v[i] > max[i] || v[i] < min[i])
            {
                return false;
            }
        }
        return true;
    }
};

#endif