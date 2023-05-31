#ifndef OBB_H
#define OBB_H

#include "Shape.h"

class OBB : public Shape
{
public:
    Transform t;

    OBB() {}
    OBB(const Transform& _t) : t(_t) {}

};

#endif