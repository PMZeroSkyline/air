#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"
#include "Gameplay/Object/Actor.h"
#include "Physic/Collision/Collision.h"
#include "Physic/Casting/ShapeCasting.h"

class CollisionComponent : public Component
{
public:
    Transform worldTransform;
    shared_ptr<Collision> collision;
    
    virtual void Message(const string& msg) override
    {
        if (msg == "ResetWorldMatrix")
        {
            worldTransform = Transform(((Actor*)owner)->worldMatrix);
        }
    }
    bool Query()
    {
        Actor* root = ((Actor*)owner)->GetRoot<Actor>();
        bool isIntersect = false;
        root->EachNode<Actor>([this, &isIntersect](Actor* curr){
            CollisionComponent* cc = curr->GetComponent<CollisionComponent>();
            if (cc)
            {
                if (cc->collision->Query(collision))
                {
                    isIntersect = true;
                    return true;
                }
            }
            return false;
        });
        return isIntersect;
    }
};

#endif