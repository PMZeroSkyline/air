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
    CameraComponent* cameraComponent = nullptr;
    Window* window;
    Role()
    {
        cameraComponent = AddComponent<CameraComponent>();
    }
    void Load(const string& path)
    {
        //scenesComponent->Load(path);
    }
    virtual void Start() override
    {
        Actor::Start();
        window = GetCurrentWindowContext();
    }
    virtual void Tick() override
    {
        Actor::Tick();
        
        vec3 dir;
        if (window->keys[GLFW_KEY_W].pressing)
        {
            dir += GetForwardVector();
        }
        if (window->keys[GLFW_KEY_S].pressing)
        {
            dir -= GetForwardVector();
        }
        if (window->keys[GLFW_KEY_D].pressing)
        {
            dir += GetRightVector();
        }
        if (window->keys[GLFW_KEY_A].pressing)
        {
            dir -= GetRightVector();
        }
        if (window->keys[GLFW_KEY_E].pressing)
        {
            dir += GetUpVector();
        }
        if (window->keys[GLFW_KEY_Q].pressing)
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