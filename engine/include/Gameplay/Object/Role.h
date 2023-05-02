#ifndef ROLE_H
#define ROLE_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "OS/Window/Window.h"

class AnimationState : public Node
{
public:
    KEY key = (KEY)-1;
    vector<string> tags;
    string animName;
    int depth = -1;
    void Setup()
    {
        depth = GetStringIndent(name);
        istringstream iss(name);
        string mark;
        iss >> mark >> animName;
        size_t keyPos = mark.find_first_of("]");
        if (keyPos != string::npos)
        {
            string keyStr = mark.substr(1, keyPos-1);
            if (keyMap.find(keyStr) != keyMap.end())
            {
                key = keyMap[keyStr];
            }
            else
            {
                LOG("failed to find anim key code !")
            }
        }
        size_t tagPos = mark.find_first_of("(");
        if (tagPos != string::npos)
        {
            string tagStr = mark.substr(tagPos+1, mark.size()-tagPos-2);
            SplitToVector(tagStr, ",", tags);
        }
    }
};

class AnimationsComponent : public Component
{
public:

    Window* window = GetCurrentWindowContext();
    vec3 dir;

    vector<AnimationInstance>* animationInstances;
    shared_ptr<AnimationState> root;

    AnimationInstance* animInst = nullptr;
    AnimationState* animState = nullptr;
    
    void Load(const string& path)
    {
        root = TreeFileParse<AnimationState>(path);
        RForEachNode<AnimationState>(root.get(), [](AnimationState* curr){
            curr->Setup();
        });
    }
    void Play(const string& name)
    {
        for_each(animationInstances->begin(), animationInstances->end(), [&name, this](AnimationInstance& curr){
            if (curr.animation->name == name)
            {
                curr.weight = 1.f;
                animInst = &curr;
            }
            else
            {
                curr.weight = 0.f;
            }
        });
    }
    void Tick() override
    {
        if (!animInst)
        {
            Play("idle");
        }
        if (dir.length() >= .1f && animInst->animation->name != "run")
        {
            Play("run");
        }
        else if(dir.length() < .1f && animInst->animation->name != "idle")
        {
            Play("idle");
        }
        animInst->time += window->deltaTime;
        if (animInst->time > animInst->animation->max)
        {
            animInst->time = animInst->animation->min;
        }
        ((Actor*)owner)->ResetWorldMatrix();
    }
};

class Role : public Actor
{
public:
    Actor* aCamArm = AddChild<Actor>();
    Actor* aCam = aCamArm->AddChild<Actor>();
    CameraComponent* camComp = aCam->AddComponent<CameraComponent>();
    Actor* aMesh = AddChild<Actor>();
    ScenesComponent* sMesh = aMesh->AddComponent<ScenesComponent>();
    AnimationsComponent* cAnim = AddComponent<AnimationsComponent>();

    vec3 dir;

    Role()
    {
        aCam->localTransform.translation = vec3(-5.f, 0.f, 0.f);
        aCam->localTransform.rotation = EulerToQuat(0.f, 0.f, -90.f);
        aCamArm->localTransform.translation = vec3(0.f, 0.f, 1.5f);
        aCamArm->localTransform.rotation = EulerToQuat(0.f, 0.f, 0.f);
        aMesh->localTransform.rotation = EulerToQuat(0.f, 0.f, 90.f);
        sMesh->Load("vroid/vroid.gltf");
        sMesh->FieldExpand();
        cAnim->animationInstances = &sMesh->animationInstances;
        cAnim->Load("anim/anim.md");
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
        wArmEul.z -= window->mouseCursor.deltaPos.x;
        wArmTrans.rotation = EulerToQuat(wArmEul);
        aCamArm->SetWorldMatrix(wArmTrans.ToMatrix());

        // set arm local y
        vec3 lArmEul = QuatToEuler(aCamArm->localTransform.rotation);
        lArmEul.y += window->mouseCursor.deltaPos.y;
        lArmEul.y = clamp(lArmEul.y, -70.f, 70.f);
        aCamArm->localTransform.rotation = EulerToQuat(lArmEul);
        aCamArm->ResetWorldMatrix(true);

        SetWorldRotation(EulerToQuat(0.f, 0.f, 30.f));

        //if (window->keys[KEY::C].pressDown)
        {
            for (int i = 0; i < sMesh->animationInstances.size(); i++)
            {
                if (sMesh->animationInstances[i].animation->name == "hit2")
                {
                    sMesh->animationInstances[i].weight = 1.f;
                    sMesh->animationInstances[i].time += window->deltaTime;
                    if (sMesh->animationInstances[i].time >= sMesh->animationInstances[0].animation->max)
                    {
                        sMesh->animationInstances[i].time = 0.f;
                    }
                }
                else
                {
                    sMesh->animationInstances[i].weight = 0.f;
                }
            }
            
            
            ResetRoot(this);
        }
        

        
        // dir = vec3();
        // if (window->keys[KEY::W].pressing)
        // {
        //     dir += aCamArm->GetForwardVector();
        // }
        // if (window->keys[KEY::S].pressing)
        // {
        //     dir -= aCamArm->GetForwardVector();
        // }
        // if (window->keys[KEY::D].pressing)
        // {
        //     dir -= aCamArm->GetRightVector();
        // }
        // if (window->keys[KEY::A].pressing)
        // {
        //     dir += aCamArm->GetRightVector();
        // }
        // dir.z = 0.f;
        // if (dir.length() > 0.1f)
        // {
        //     dir = dir.normalize();
        //     Transform wTrans = Transform(worldMatrix);

        //     // rotate actor and clear arm local rotation
        //     wTrans.rotation = EulerToQuat(0.f, 0.f, wArmEul.z);
        //     vec3 lArmEul = QuatToEuler(aCamArm->localTransform.rotation);
        //     aCamArm->localTransform.rotation = EulerToQuat(lArmEul.x, lArmEul.y, 0.f);

        //     // add world location offset
        //     wTrans.translation += dir * window->deltaTime * 5.f;
        //     SetWorldMatrix(wTrans.ToMatrix());

        //     // Setup mesh rotation
        //     Transform wMeshTrans = Transform(aMesh->worldMatrix);
        //     float wAngleZ = atan2(dir.y, dir.x);
        //     wMeshTrans.rotation = EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f);
        //     aMesh->SetWorldMatrix(wMeshTrans.ToMatrix());
        // }
        // cAnim->dir = dir;
    }
};

#endif