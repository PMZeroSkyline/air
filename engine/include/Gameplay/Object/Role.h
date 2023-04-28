#ifndef ROLE_H
#define ROLE_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "OS/Window/Window.h"

class Role : public Actor
{
public:
    //https://learnopengl.com/Getting-started/Coordinate-Systems
    Actor* aCamArm = AddChild<Actor>();
    Actor* aCam = aCamArm->AddChild<Actor>();
    CameraComponent* camComp = aCam->AddComponent<CameraComponent>();
    Actor* aMesh = AddChild<Actor>();
    ScenesComponent* sMesh = aMesh->AddComponent<ScenesComponent>();
    Window* win = GetCurrentWindowContext();
    Role()
    {
        aCam->localTransform.translation = vec3(-5.f, 0.f, 0.f);
        aCam->localTransform.rotation = EulerToQuat(0.f, 0.f, -90.f);
        aCamArm->localTransform.translation = vec3(0.f, 0.f, 1.5f);
        aCamArm->localTransform.rotation = EulerToQuat(0.f, 0.f, 0.f);
        aMesh->localTransform.rotation = EulerToQuat(0.f, 0.f, 90.f);
        sMesh->Load("vroid/anim/anim.gltf");
    }
    virtual void Start() override
    {
        Actor::Start();
    }
    virtual void Tick() override
    {
        Actor::Tick();

        vec3 eulArmRot = QuatToEuler(aCamArm->localTransform.rotation);
        eulArmRot.y += win->mouseCursor.deltaPos.y;
        eulArmRot.z += -win->mouseCursor.deltaPos.x;
        eulArmRot.y = clamp(eulArmRot.y, -80.f, 80.f);
        aCamArm->localTransform.rotation = EulerToQuat(eulArmRot);
        ResetWorldMatrix(true);
        
        vec3 dir;
        if (win->keys[KEY::W].pressing)
        {
            dir += aCam->GetRightVector();
        }
        if (win->keys[KEY::S].pressing)
        {
            dir -= aCam->GetRightVector();
        }
        if (win->keys[KEY::D].pressing)
        {
            dir += aCam->GetForwardVector();
        }
        if (win->keys[KEY::A].pressing)
        {
            dir -= aCam->GetForwardVector();
        }
        if (win->keys[KEY::E].pressing)
        {
            dir += GetUpVector();
        }
        if (win->keys[KEY::Q].pressing)
        {
            dir -= GetUpVector();
        }
        
        dir.z = 0;
        if (dir.length() > 0.1f)
        {



            dir = dir.normalize();
            localTransform.translation += dir * .1f;
            ResetWorldMatrix(true);
            
            Transform tMesh = aMesh->worldMatrix;
            float theta = atan2(dir.y, dir.x);
            tMesh.rotation = EulerToQuat(0.f, 0.f, degrees(theta)+90.f);
            aMesh->SetWorldMatrix(tMesh.ToMatrix());
        }

        if (dir.length() < 0.1f)
        {
            sMesh->animationInstances[0].weight = 1.f;
            sMesh->animationInstances[1].weight = 0.f;
            sMesh->animationInstances[0].time = sMesh->animationInstances[0].time + win->deltaTime * 2.f;
            if (sMesh->animationInstances[0].time > sMesh->animationInstances[0].animation->max)
            {
                sMesh->animationInstances[0].time = 0.f;
            }
            ResetWorldMatrix(true);
        }
        else
        {
            sMesh->animationInstances[1].weight = 1.f;
            sMesh->animationInstances[0].weight = 0.f;
            sMesh->animationInstances[1].time = sMesh->animationInstances[1].time + win->deltaTime * 2.f;
            if (sMesh->animationInstances[1].time > sMesh->animationInstances[1].animation->max)
            {
                sMesh->animationInstances[1].time = 0.f;
            }
            ResetWorldMatrix(true);
        }
    }
};

#endif