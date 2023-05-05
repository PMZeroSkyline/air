#ifndef ROLE_H
#define ROLE_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "OS/Window/Window.h"

class AnimationState : public Node
{
public:
    AnimationInstance* animationInstance = nullptr;

    int depth = -1;
    bool isRef = false;

    KEY key = (KEY)-1;
    float time = 0.f;
    vector<string> tags;

    bool isKeyReady = false;

    void Setup(vector<AnimationInstance>* animationInstances, map<string, AnimationState*>* stateMap)
    {
        depth = GetStringIndent(name);
        istringstream iss(name);
        string mark, type;
        iss >> type >> mark >> name;
        isRef = type == "*";
        if (!isRef)
        {
            (*stateMap)[name] = this;
        }
        if (string keyStr = GetBracketContent(mark, "[]"); keyStr != "")
        {
            if (keyMap.find(keyStr) != keyMap.end())
            {
                key = keyMap[keyStr];
            }
        }
        if (string timeStr = GetBracketContent(mark, "()"); timeStr != "")
        {
            time = atof(timeStr.c_str());
        }
        if (string tagStr = GetBracketContent(mark, "{}"); tagStr != "")
        {
            SplitToVector(tagStr, ",", tags);
        }
        vector<AnimationInstance>::iterator found = find_if(animationInstances->begin(), animationInstances->end(), [this](AnimationInstance& curr){
            return curr.animation->name == name;
        });
        if (found != animationInstances->end())
        {
            animationInstance = &(*found);
            if (animationInstance)
            {
                if (name == "idle" || name == "run")
                {
                    animationInstance->isLoop = true;
                }
            }
        }
    }
    virtual bool Check(AnimationState* currentState, vec3* dir)
    {
        if (key == (KEY)-1)
        {
            isKeyReady = true;
        }
        else
        {
            if (window->keys[key].pressDown)
            {
                isKeyReady = true;
            }
        }
        bool isTimeReady = false;
        if (time == 0)
        {
            isTimeReady = true;
        }
        else
        {
            if (currentState)
            {
                float currRate = currentState->animationInstance->time / currentState->animationInstance->animation->max;
                if (currRate >= time)
                {
                    isTimeReady = true;
                }
            }
        }
        bool isTagReady = true;
        for (int i = 0; i < tags.size(); i++)
        {
            const string& tag = tags[i];
            if (tag == "stop" && dir->length() > 0.1f)
            {
                isTagReady = false;
            }
            else if (tag == "move" && dir->length() < 0.1f)
            {
                isTagReady = false;
            }
        }
        return isKeyReady && isTimeReady && isTagReady;
    }
};
class AnimationStateMacine : public Component
{
public:
    vector<AnimationInstance>* animationInstances = nullptr;
    shared_ptr<AnimationState> rootState;
    map<string, AnimationState*> stateMap;
    AnimationState* currentState = nullptr;
    vec3* dir = nullptr;
    Actor* aMesh = nullptr;
    void Load(const string& path)
    {
        rootState = TreeFileParse<AnimationState>(path);
        rootState->RForEach<AnimationState>([this](AnimationState* curr){
            curr->Setup(animationInstances, &stateMap);
        });
        currentState = rootState.get();
    }
    void Switch(AnimationState* state)
    {
        state->isKeyReady = false;
        if (state->name == currentState->name)
        {
            return;
        }
        if (map<string, AnimationState*>::iterator found = stateMap.find(state->name); found != stateMap.end())
        {
            currentState = found->second;
            for_each(animationInstances->begin(), animationInstances->end(), [this](AnimationInstance& curr){
                curr.weight = 0.f;
            });
            if (currentState->animationInstance)
            {
                currentState->animationInstance->time = 0.f;
                currentState->animationInstance->weight = 1.f;
            }
        }
    }
    void Tick() override
    {
        for (int i = currentState->children.size()-1; i >= 0; i--)
        {
            AnimationState* state = (AnimationState*)currentState->children[i];
            if (state->Check(currentState, dir))
            {
                if (dir->length() > 0.1f)
                {
                    Transform wMeshTrans = Transform(aMesh->worldMatrix);
                    float wAngleZ = atan2(dir->y, dir->x);
                    wMeshTrans.rotation = EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f);
                    aMesh->SetWorldMatrix(wMeshTrans.ToMatrix());
                }
                Switch(state);
                break;
            }
        }
        if (currentState->animationInstance)
        {
            currentState->animationInstance->time += window->deltaTime;
            if (currentState->animationInstance->isLoop && currentState->animationInstance->time >= currentState->animationInstance->animation->max)
            {
                currentState->animationInstance->time = 0.f;
            }
            ((Actor*)owner)->ResetWorldMatrix(false, (Actor*)owner);
        }
        
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
    AnimationStateMacine* cAnimMachine = AddComponent<AnimationStateMacine>();
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
        cAnimMachine->animationInstances = &sMesh->animationInstances;
        cAnimMachine->Load("anim/anim1.md");
        cAnimMachine->dir = &dir;
        cAnimMachine->aMesh = aMesh;
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

        dir = vec3();
        if (window->keys[KEY::W].pressing)
        {
            dir += aCamArm->GetForwardVector();
        }
        if (window->keys[KEY::S].pressing)
        {
            dir -= aCamArm->GetForwardVector();
        }
        if (window->keys[KEY::D].pressing)
        {
            dir -= aCamArm->GetRightVector();
        }
        if (window->keys[KEY::A].pressing)
        {
            dir += aCamArm->GetRightVector();
        }
        dir.z = 0.f;
        if (dir.length() > 0.1f && cAnimMachine->currentState->name == "run")
        {
            dir = dir.normalize();
            Transform wTrans = Transform(worldMatrix);

            // rotate actor and clear arm local rotation
            wTrans.rotation = EulerToQuat(0.f, 0.f, wArmEul.z);
            vec3 lArmEul = QuatToEuler(aCamArm->localTransform.rotation);
            aCamArm->localTransform.rotation = EulerToQuat(lArmEul.x, lArmEul.y, 0.f);

            // add world location offset
            wTrans.translation += dir * window->deltaTime * 5.f;
            SetWorldMatrix(wTrans.ToMatrix());

            // Setup mesh rotation
            Transform wMeshTrans = Transform(aMesh->worldMatrix);
            float wAngleZ = atan2(dir.y, dir.x);
            wMeshTrans.rotation = EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f);
            aMesh->SetWorldMatrix(wMeshTrans.ToMatrix());
            //aMesh->SetWorldRotation(EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f));
        }
    }
};

#endif