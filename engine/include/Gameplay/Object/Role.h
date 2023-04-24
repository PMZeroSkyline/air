#ifndef ROLE_H
#define ROLE_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "OS/Window/Window.h"

class Role : public Actor
{
public:
    Actor* cameraArmActor = nullptr;
    Actor* cameraActor = nullptr;
    CameraComponent* cameraComponent = nullptr;
    Actor* meshActor = nullptr;
    ScenesComponent* scenesComponent = nullptr;
    Window* window;
    Role()
    {
        cameraArmActor = AddChild<Actor>();
        cameraArmActor->localTransform.translation = vec3(0,1.5,0);
        cameraArmActor->localTransform.rotation = EulerToQuat(vec3(0,90,0));
        cameraActor = cameraArmActor->AddChild<Actor>();
        cameraActor->localTransform.translation = vec3(0,0,5);
        cameraComponent = cameraActor->AddComponent<CameraComponent>();
        meshActor = AddChild<Actor>();
        meshActor->localTransform.rotation = EulerToQuat(vec3(0,-90,0));
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

        Transform worldTransform = Transform(worldMatrix);
        worldTransform.rotation = EulerToQuat(vec3(0, window->time*30.f, 0));
        SetWorldMatrix(worldTransform.ToMatrix());

        // worldTransform = Transform(worldMatrix);
        // vec3 dir;
        
        // if (window->keys[GLFW_KEY_W].pressing)
        // {
        //     dir += GetForwardVector();
        // }
        // if (window->keys[GLFW_KEY_S].pressing)
        // {
        //     dir -= GetForwardVector();
        // }
        // worldTransform.translation += dir;
        // SetWorldMatrix(worldTransform.ToMatrix());
    }
};

#endif