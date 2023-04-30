#ifndef ROLE_H
#define ROLE_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "OS/Window/Window.h"



class Role : public Actor
{
public:
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
        sMesh->Load("vroid/vroid.gltf");
    }
    virtual void Start() override
    {
        Actor::Start();
    }
    virtual void Tick() override
    {
        Actor::Tick();

        // set arm world z
        Transform wArmTrans = Transform(aCamArm->worldMatrix);
        vec3 wArmEul = QuatToEuler(wArmTrans.rotation);
        wArmEul.z -= win->mouseCursor.deltaPos.x;
        wArmTrans.rotation = EulerToQuat(wArmEul);
        aCamArm->SetWorldMatrix(wArmTrans.ToMatrix());

        // set arm local y
        vec3 lArmEul = QuatToEuler(aCamArm->localTransform.rotation);
        lArmEul.y += win->mouseCursor.deltaPos.y;
        lArmEul.y = clamp(lArmEul.y, -70.f, 70.f);
        aCamArm->localTransform.rotation = EulerToQuat(lArmEul);
        aCamArm->ResetWorldMatrix(true);

        vec3 dir;
        if (win->keys[KEY::W].pressing)
        {
            dir += aCamArm->GetForwardVector();
        }
        if (win->keys[KEY::S].pressing)
        {
            dir -= aCamArm->GetForwardVector();
        }
        if (win->keys[KEY::D].pressing)
        {
            dir -= aCamArm->GetRightVector();
        }
        if (win->keys[KEY::A].pressing)
        {
            dir += aCamArm->GetRightVector();
        }
        dir.z = 0.f;
        if (dir.length() > 0.1f)
        {
            dir = dir.normalize();
            Transform wTrans = Transform(worldMatrix);

            // rotate actor and clear arm local rotation
            wTrans.rotation = EulerToQuat(0.f, 0.f, wArmEul.z);
            vec3 lArmEul = QuatToEuler(aCamArm->localTransform.rotation);
            aCamArm->localTransform.rotation = EulerToQuat(lArmEul.x, lArmEul.y, 0.f);

            // add world location offset
            wTrans.translation += dir * .02f;
            SetWorldMatrix(wTrans.ToMatrix());

            // Setup mesh rotation
            Transform wMeshTrans = Transform(aMesh->worldMatrix);
            float wAngleZ = atan2(dir.y, dir.x);
            wMeshTrans.rotation = EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f);
            aMesh->SetWorldMatrix(wMeshTrans.ToMatrix());

            if (!sMesh->animDemo.currentAnimationInstance || sMesh->animDemo.currentAnimationInstance->animation->name != "run")
            {
                sMesh->animDemo.Play("run");
            }
        }
        else
        {
            if (!sMesh->animDemo.currentAnimationInstance || sMesh->animDemo.currentAnimationInstance->animation->name != "idle")
            {
                sMesh->animDemo.Play("idle");
            }
        }
        ResetWorldMatrix(true);
        sMesh->animDemo.Tick();
    }
};

#endif