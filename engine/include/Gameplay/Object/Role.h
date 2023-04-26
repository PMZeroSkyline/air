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
        cam->localTransform.translation = vec3(0.f, 0.f, 0.f);
        camArm->localTransform.translation = vec3(0.f, 0.f, 0.f);
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