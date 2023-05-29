#ifndef COLLISION_H
#define COLLISION_H

#include "Physic/Shape/OBB.h";
#include "Physic/Shape/Capsule.h";
#include "Physic/Shape/Sphere.h";
#include "Physic/Casting/ShapeCasting.h"

class Collision
{
public:
    shared_ptr<Shape> shape;

    bool Query(shared_ptr<Collision> target)
    {
        if (typeid(*shape.get()) == typeid(Capsule))
        {
            if (typeid(*shape.get()) == typeid(OBB))
            {
                if (IsIntersectOBBCapsule((OBB*)target->shape.get(), (Capsule*)shape.get()))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif