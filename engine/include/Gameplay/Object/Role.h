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
        sMesh->Load("idle_zup/idle.gltf");
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
        if (win->keys[GLFW_KEY_W].pressing)
        {
            dir += aCam->GetRightVector();
        }
        if (win->keys[GLFW_KEY_S].pressing)
        {
            dir -= aCam->GetRightVector();
        }
        if (win->keys[GLFW_KEY_D].pressing)
        {
            dir += aCam->GetForwardVector();
        }
        if (win->keys[GLFW_KEY_A].pressing)
        {
            dir -= aCam->GetForwardVector();
        }
        if (win->keys[GLFW_KEY_E].pressing)
        {
            dir += GetUpVector();
        }
        if (win->keys[GLFW_KEY_Q].pressing)
        {
            dir -= GetUpVector();
        }
        if (dir.length() > 0.5f)
        {
            dir.z = 0;
            dir = dir.normalize();
            localTransform.translation += dir;
            ResetWorldMatrix(true);
            
            Transform tMesh = aMesh->worldMatrix;
            float theta = atan2(dir.y, dir.x);
            tMesh.rotation = EulerToQuat(0.f, 0.f, degrees(theta)+90.f);
            aMesh->SetWorldMatrix(tMesh.ToMatrix());
        }
        
    }
};

#endif