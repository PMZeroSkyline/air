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
        cameraArmActor->localTransform.rotation = EulerToQuat(vec3(0,180,0));
        cameraActor = cameraArmActor->AddChild<Actor>();
        cameraActor->localTransform.translation = vec3(0,0,5);
        cameraComponent = cameraActor->AddComponent<CameraComponent>();
        meshActor = AddChild<Actor>();
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
        localTransform.rotation = EulerToQuat(vec3(0, window->mouseCursor.pos.x, 0));
    }
};

#endif