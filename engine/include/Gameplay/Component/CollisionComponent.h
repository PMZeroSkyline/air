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

    bool IsIntersectQuery(const mat4& expectMatrix)
    {
        Actor* root = owner->GetRoot<Actor>();
        bool result = false;
        root->ForEachNode<Actor>([this, &result, &expectMatrix](Actor* curr){
            CollisionComponent* cCompare = curr->GetComponent<CollisionComponent>();
            if (!cCompare || cCompare == this)
            {
                return false;
            }
            if (IsIntersect(shape.get(), expectMatrix, cCompare->shape.get(), ((Actor*)cCompare->owner)->worldMatrix))
            {
                result = true;
                return true;
            }
            return false;
        });
        return result;
    }
};

#endif