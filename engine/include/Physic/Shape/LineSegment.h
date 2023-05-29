#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "Shape.h"

class LineSegment : public Shape
{
public:
    vec3 a;
    vec3 b;

    LineSegment(vec3 _a, vec3 _b) : a(_a), b(_b) {}


};

#endif