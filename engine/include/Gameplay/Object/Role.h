#ifndef ROLE_H
#define ROLE_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Gameplay/Component/AnimationPlayerComponent.h"
#include "Gameplay/World/WorldGenerate.h"
#include "Physic/Casting/Casting.h"
#include "Gameplay/Component/CollisionComponent.h"


// class AnimationState : public Node
// {
// public:
//     int depth = -1;
//     bool isRef = false;
//     AnimationInstance* animInst = nullptr;
//     KEY key = (KEY)-1;
//     float percent = 0.f;
//     vector<string> tags;
//     bool isKeyReady = false;
//     void Setup(vector<AnimationInstance>* animationInstances, map<string, AnimationState*>* stateMap)
//     {
//         depth = GetStringIndent(name);
//         istringstream iss(name);
//         string mark, type;
//         iss >> type >> mark >> name;
//         isRef = type == "*";
//         if (!isRef)
//         {
//             (*stateMap)[name] = this;
//         }
//         if (string keyStr = GetBracketContent(mark, "[]"); keyStr != "")
//         {
//             if (keyMap.find(keyStr) != keyMap.end())
//             {
//                 key = keyMap[keyStr];
//             }
//         }
//         if (string percentStr = GetBracketContent(mark, "()"); percentStr != "")
//         {
//             percent = atof(percentStr.c_str());
//         }
//         if (string tagStr = GetBracketContent(mark, "{}"); tagStr != "")
//         {
//             SplitToVector(tagStr, ",", tags);
//         }
//         vector<AnimationInstance>::iterator found = find_if(animationInstances->begin(), animationInstances->end(), [this](AnimationInstance& curr){
//             return curr.animation->name == name;
//         });
//         if (found != animationInstances->end())
//         {
//             animInst = &(*found);
//             if (animInst)
//             {
//                 if (name == "idle" || name == "run")
//                 {
//                     animInst->isLoop = true;
//                 }
//             }
//         }
//     }
//     virtual bool Check(AnimationState* currentState, vec3* dir)
//     {
//         if (key == (KEY)-1)
//         {
//             isKeyReady = true;
//         }
//         else
//         {
//             if (window->keys[key].pressDown)
//             {
//                 isKeyReady = true;
//             }
//         }
//         bool isPercentReady = false;
//         if (percent == 0)
//         {
//             isPercentReady = true;
//         }
//         else
//         {
//             if (currentState)
//             {
//                 float currRate = currentState->animInst->time / currentState->animInst->animation->max;
//                 if (currRate >= percent)
//                 {
//                     isPercentReady = true;
//                 }
//             }
//         }
//         bool isTagReady = true;
//         for (int i = 0; i < tags.size(); i++)
//         {
//             const string& tag = tags[i];
//             if (tag == "stop" && dir->length() > 0.1f)
//             {
//                 isTagReady = false;
//             }
//             else if (tag == "move" && dir->length() < 0.1f)
//             {
//                 isTagReady = false;
//             }
//         }
//         return isKeyReady && isPercentReady && isTagReady;
//     }
// };
// class AnimationStateMacine : public Component
// {
// public:
//     vector<AnimationInstance>* animationInstances = nullptr;
//     shared_ptr<AnimationState> rootState;
//     map<string, AnimationState*> stateMap;
//     AnimationState* currentState = nullptr;
//     vec3* dir = nullptr;
//     Actor* aMesh = nullptr;
//     void Load(const string& path)
//     {
//         rootState = TreeFileParse<AnimationState>(path);
//         rootState->REachNode<AnimationState>([this](AnimationState* curr){
//             curr->Setup(animationInstances, &stateMap);
//         });
//         currentState = rootState.get();
//     }
//     void Switch(AnimationState* state)
//     {
//         state->isKeyReady = false;
//         if (state->name == currentState->name)
//         {
//             return;
//         }
//         if (map<string, AnimationState*>::iterator found = stateMap.find(state->name); found != stateMap.end())
//         {
//             currentState = found->second;
//             for_each(animationInstances->begin(), animationInstances->end(), [this](AnimationInstance& curr){
//                 curr.weight = 0.f;
//             });
//             if (currentState->animInst)
//             {
//                 currentState->animInst->time = 0.f;
//                 currentState->animInst->weight = 1.f;
//             }
//         }
//     }
//     void Tick() override
//     {
//         if (!currentState)
//         {
//             return;
//         }
        
//         for (int i = currentState->children.size()-1; i >= 0; i--)
//         {
//             AnimationState* state = (AnimationState*)currentState->children[i];
//             if (state->Check(currentState, dir))
//             {
//                 if (dir->length() > 0.1f)
//                 {
//                     Transform wMeshTrans = Transform(aMesh->worldMatrix);
//                     float wAngleZ = atan2(dir->y, dir->x);
//                     wMeshTrans.rotation = EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f);
//                     aMesh->SetWorldMatrix(wMeshTrans.ToMatrix());
//                 }
//                 Switch(state);
//                 break;
//             }
//         }
//         if (currentState->animInst)
//         {
//             currentState->animInst->time += window->deltaTime;
//             if (currentState->animInst->isLoop && currentState->animInst->time >= currentState->animInst->animation->max)
//             {
//                 currentState->animInst->time = 0.f;
//             }
//             ((Actor*)owner)->ResetWorldMatrix(false, (Actor*)owner);
//         }
        
//     }
// };
class Role : public Actor
{
public:

    // camera
    Actor* aCamArm = AddChild<Actor>();
    Actor* aCam = aCamArm->AddChild<Actor>();
    CameraComponent* camComp = aCam->AddComponent<CameraComponent>();

    // mesh
    Actor* aModel = AddChild<Actor>();
    ScenesComponent* sModel = aModel->AddComponent<ScenesComponent>();
    AnimationPlayerComponent* cPlayer = AddComponent<AnimationPlayerComponent>();
    CollisionComponent* cCollision = aModel->AddComponent<CollisionComponent>();

    // motion
    vec3 dir;

    Role()
    {
        aCam->localTransform.translation = vec3(-5.f, 0.f, 0.f);
        aCam->localTransform.rotation = EulerToQuat(0.f, 0.f, -90.f);
        aCamArm->localTransform.translation = vec3(0.f, 0.f, 1.5f);
        aCamArm->localTransform.rotation = EulerToQuat(0.f, 0.f, 0.f);
        aModel->localTransform.rotation = EulerToQuat(0.f, 0.f, 90.f);
        
        GenCapsuleMan(aModel);
        shared_ptr<Capsule> sCapsule = make_shared<Capsule>();
        cCollision->a = sCapsule;

        //mc->mesh->primitives[0]->material->faceMode = GLFaceMode::LINE;
        //sModel->Load("model/blender/aurelia/aurelia.gltf");
        //sModel->Load("model/blender/mixamo/ybot_idle/ybot_idle.gltf");
        //sModel->FieldExpand();
        //cPlayer->animInsts = &sModel->animationInstances;
        //cPlayer->Play("idle", true);
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

        dir = vec3(0.f);
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
        if (dir.length() > 0.1f)
        {
            dir = dir.normalize();
            Transform wTrans = Transform(worldMatrix);

            // rotate actor and clear arm local rotation
            wTrans.rotation = EulerToQuat(0.f, 0.f, wArmEul.z);
            vec3 lArmEul = QuatToEuler(aCamArm->localTransform.rotation);
            aCamArm->localTransform.rotation = EulerToQuat(lArmEul.x, lArmEul.y, 0.f);

            // add world location offset
            // * window->deltaTime * 10.f
            SetWorldMatrix(cCollision->GetMoveMatrix(dir * window->deltaTime * 10.f, wTrans));

            // Setup mesh rotation
            Transform wMeshTrans = Transform(aModel->worldMatrix);
            float wAngleZ = atan2(dir.y, dir.x);
            wMeshTrans.rotation = EulerToQuat(0.f, 0.f, degrees(wAngleZ)+90.f);
            aModel->SetWorldMatrix(wMeshTrans.ToMatrix());
            
            


            // float dis = QueryPointToLineSegmentDistance(LineSegment(vec3(-.707f, 0.707f, 0.f), vec3(.707f, -.707f, 0.f)), wMeshTrans.translation);
            // string s = (dis <= 1.f) ? "true" : "false";
            // LOG(s)

            // float sd = SignedDistanceCapsuleFromPoint(Capsule(vec3(-.707f, 0.707f, 0.f), vec3(.707f, -.707f, 0.f), 1.f), wMeshTrans.translation);
            // LOG(sd)
        }
    }
};

#endif