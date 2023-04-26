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
    Actor* cameraArmActor = nullptr;
    Actor* cameraActor = nullptr;
    CameraComponent* cameraComponent = nullptr;
    Actor* meshActor = nullptr;
    ScenesComponent* scenesComponent = nullptr;
    Window* window;
    Role()
    {
        cameraArmActor = AddChild<Actor>();
        cameraArmActor->localTransform.translation = vec3(0,0,1.5);
        cameraArmActor->localTransform.rotation = EulerToQuat(vec3(0,0,-90));
        cameraActor = cameraArmActor->AddChild<Actor>();
        cameraActor->localTransform.translation = vec3(0,5,0);
        cameraComponent = cameraActor->AddComponent<CameraComponent>();
        meshActor = AddChild<Actor>();
        meshActor->localTransform.rotation = EulerToQuat(vec3(0,0,90));
        scenesComponent = meshActor->AddComponent<ScenesComponent>();
        window = GetCurrentWindowContext();
    }
    void Load(const string& path)
    {
        scenesComponent->Load(path);
    }
    virtual void Start() override
    {
        Actor::Start();
    }
    virtual void Tick() override
    {
        Actor::Tick();
        vec3 eul = QuatToEuler(cameraArmActor->localTransform.rotation);
        eul.y += window->deltaTime*30.f;
        //LOG(eul.y)
        //cameraArmActor->localTransform.rotation = EulerToQuat(10.f, 0.f, eul.y);

        ResetWorldMatrix(true);
        
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
        if (dir.length() > 0.5f)
        {
            dir = dir.normalize();
        }
        localTransform.translation += dir;

        ResetWorldMatrix(true);
    }
};

#endif