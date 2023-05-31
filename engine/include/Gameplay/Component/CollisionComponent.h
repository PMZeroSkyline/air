#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"
#include "Gameplay/Object/Actor.h"
#include "Physic/Collision/Collision.h"
#include "Physic/Casting/Casting.h"

class CollisionComponent : public Component
{
public:
    shared_ptr<Shape> shape;

    float IsIntersect(const mat4& expectMatrix)
    {
        Actor* root = owner->GetRoot<Actor>();
        float result = 0.f;
        root->ForEachNode<Actor>([this, &result, &expectMatrix](Actor* curr){
            CollisionComponent* cCompare = curr->GetComponent<CollisionComponent>();
            if (!cCompare || cCompare == this)
            {
                return false;
            }
            float sd = SDF(shape.get(), expectMatrix, cCompare->shape.get(), ((Actor*)cCompare->owner)->worldMatrix);
            if (sd <= 0.f)
            {
                result = sd;
                return true;
            }
            return false;
        });
        return result;
    }
};

#endif