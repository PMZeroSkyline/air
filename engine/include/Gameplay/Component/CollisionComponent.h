#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"
#include "Gameplay/Object/Actor.h"
#include "Physic/Collision/Collision.h"
#include "Physic/Casting/Casting.h"

class CollisionComponent : public Component
{
public:
    shared_ptr<Shape> a;

    float SD(shared_ptr<Shape> b, const mat4& aw, const mat4& bw)
    {
        if (!a || !b)
        {
            return 0.f;
        }
        if (IsSharedType(a, Sphere))
        {
            if (IsSharedType(b, OBB))
            {
                Sphere x = Transf((Sphere*)a.get(), aw);
                OBB y = Transf((OBB*)b.get(), bw);
                return SDF(&x, &y);
            }
            if (IsSharedType(b, Sphere))
            {
                Sphere x = Transf((Sphere*)a.get(), aw);
                Sphere y = Transf((Sphere*)b.get(), bw);
                return SDF(&x, &y);
            }
            if (IsSharedType(b, Capsule))
            {
                Sphere x = Transf((Sphere*)a.get(), aw);
                Capsule y = Transf((Capsule*)b.get(), bw);
                return SDF(&x, &y);
            }
        }
        if (IsSharedType(a, Capsule))
        {
            if (IsSharedType(b, OBB))
            {
                Capsule x = Transf((Capsule*)a.get(), aw);
                OBB y = Transf((OBB*)b.get(), bw);
                return SDF(&x, &y);
            }
            if (IsSharedType(b, Sphere))
            {
                Capsule x = Transf((Capsule*)a.get(), aw);
                Sphere y = Transf((Sphere*)b.get(), bw);
                return SDF(&y, &x);
            }
            if (IsSharedType(b, Capsule))
            {
                Capsule x = Transf((Capsule*)a.get(), aw);
                Capsule y = Transf((Capsule*)b.get(), bw);
                return SDF(&x, &y);
            }
        }
        return 0.f;
    }
    float IsIntersectQuery(const mat4& aw)
    {
        Actor* root = owner->GetRoot<Actor>();
        float result = 0.f;
        root->ForEachNode<Actor>([this, &result, &aw](Actor* curr){
            CollisionComponent* cc = curr->GetComponent<CollisionComponent>();
            if (!cc || cc == this)
            {
                return false;
            }
            float sd = SD(cc->a, aw, ((Actor*)cc->owner)->worldMatrix);
            if (sd <= 0.f)
            {
                result = sd;
                return true;
            }
            return false;
        });
        return result;
    }
    mat4 GetMoveMatrix(const vec3& dir, const Transform& wTrans)
    {
        Transform wMoveTrans = wTrans;
        wMoveTrans.translation += dir;
        mat4 moveMatrix = wMoveTrans.ToMatrix();
        if (IsIntersectQuery(moveMatrix) < 0.f)
        {
            Transform wRTrans = wTrans;
            wRTrans.translation += ToVec3(mat4(EulerToQuat(vec3(0.f, 0.f, 90.f))) * vec4(dir, 1.f));
            mat4 wRMatrix = wRTrans.ToMatrix();
            if (IsIntersectQuery(wRMatrix) >= 0.f)
            {
                return wRMatrix;
            }
            else
            {
                Transform wLTrans = wTrans;
                wLTrans.translation += ToVec3(mat4(EulerToQuat(vec3(0.f, 0.f, -90.f))) * vec4(dir, 1.f)) * window->deltaTime * 10.f;
                mat4 wLMatrix = wLTrans.ToMatrix();
                if (IsIntersectQuery(wLMatrix) >= 0.f)
                {
                    return wLMatrix;
                }
            }
        }
        else
        {
            return moveMatrix;
        }
        return wTrans.ToMatrix();
    }
};

#endif