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
    Actor* camArm = AddChild<Actor>();
    Actor* cam = camArm->AddChild<Actor>();
    CameraComponent* camComp = cam->AddComponent<CameraComponent>();
    Window* win = GetCurrentWindowContext();
    Role()
    {
        cam->localTransform.translation = vec3(-5.f, 0.f, 0.f);
        cam->localTransform.rotation = EulerToQuat(0.f, 0.f, -90.f);
        camArm->localTransform.translation = vec3(0.f, 0.f, 1.5f);
        camArm->localTransform.rotation = EulerToQuat(0.f, 0.f, 0.f);
    }
    void Load(const string& path)
    {
    }
    virtual void Start() override
    {
        Actor::Start();
    }
    virtual void Tick() override
    {
        Actor::Tick();
        vec3 armRot = QuatToEuler(camArm->localTransform.rotation);
        camArm->localTransform.rotation = EulerToQuat(vec3(0, 30.f, armRot.z+win->deltaTime*30.f));
        ResetWorldMatrix(true);
        
        vec3 dir;
        if (win->keys[GLFW_KEY_W].pressing)
        {
            dir += GetForwardVector();
        }
        if (win->keys[GLFW_KEY_S].pressing)
        {
            dir -= GetForwardVector();
        }
        if (win->keys[GLFW_KEY_D].pressing)
        {
            dir += GetRightVector();
        }
        if (win->keys[GLFW_KEY_A].pressing)
        {
            dir -= GetRightVector();
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
            dir = dir.normalize();
        }
        localTransform.translation += dir;

        ResetWorldMatrix(true);
    }
};

#endif