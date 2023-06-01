#ifndef SCENES_COMPONENT_H
#define SCENES_COMPONENT_H

#include "Component.h"
#include "Resource/Model/Scenes.h"
#include "Gameplay/Object/Actor.h"
#include "Animation/AnimationView.h"
#include "AnimationNodeComponent.h"
#include "MeshComponent.h"
#include "Animation/ArmatureInstance.h"
#include "Core/Parse/TreeFileParse.h"

class ScenesComponent : public Component
{
public:
    shared_ptr<Scenes> scenes;
    vector<Actor*> nodes;
    vector<ArmatureInstance> armatureInstances;
    vector<AnimationInstance> animationInstances;

    void Load(const string& path)
    {
        scenes = scenesWeakMap.Get(path);
        if (!scenes)
        {
            scenes = make_shared<Scenes>();
            scenes->Load(path);
            scenesWeakMap.Set(path, scenes);
        }
    }
    void NodeExpand(SceneNode* sceneNode, Actor* node)
    {           
        node->name = sceneNode->name;
        node->localTransform = sceneNode->localTransform;

        MeshComponent* meshComponent = nullptr;
        if (sceneNode->meshID != -1)
        {
            meshComponent = node->AddComponent<MeshComponent>();
            meshComponent->mesh = scenes->meshs[sceneNode->meshID];
        }
        if (sceneNode->skinID != -1)
        {
            if (!meshComponent)
            {
                meshComponent = node->AddComponent<MeshComponent>();
            }
            meshComponent->armatureInstance = &armatureInstances[sceneNode->skinID];
        }

        for (int i = 0; i < sceneNode->childrenID.size(); i++)
        {
            SceneNode* sceneNodeChild = &scenes->nodes[sceneNode->childrenID[i]];
            Actor* nodeChild = node->AddChild<Actor>();
            nodes[sceneNode->childrenID[i]] = nodeChild;
            NodeExpand(sceneNodeChild, nodeChild);
        }
    }
    void FieldExpand()
    {
        nodes.resize(scenes->nodes.size());
        armatureInstances.resize(scenes->armatures.size());
        animationInstances.resize(scenes->animations.size());

        Scene* scene = &scenes->scenes[scenes->sceneID];
        name = scene->name;
        for (int i = 0; i < scene->nodeIDs.size(); i++)
        {
            SceneNode* sceneNode = &scenes->nodes[scene->nodeIDs[i]];
            Actor* nodeChild = owner->AddChild<Actor>();
            nodes[scene->nodeIDs[i]] = nodeChild;
            NodeExpand(sceneNode, nodeChild);
        }

        for (int i = 0; i < scenes->animations.size(); i++)
        {
            shared_ptr<Animation> animation = scenes->animations[i];
            AnimationInstance* animationInstance = &animationInstances[i];

            animationInstance->animation = animation.get();

            for (int j = 0; j < animation->channels.size(); j++)
            {
                AnimationChannel* channel = &animation->channels[j];
                Actor* targetNode = nodes[channel->target.nodeID];

                AnimationNodeComponent* animationNodeComponent = targetNode->GetComponent<AnimationNodeComponent>();
                if (!animationNodeComponent)
                {
                    animationNodeComponent = targetNode->AddComponent<AnimationNodeComponent>();
                }

                AnimationView* animationView = animationNodeComponent->GetAnimationView(animationInstance);
                if (!animationView)
                {
                    animationNodeComponent->animationViews.push_back(AnimationView());
                    animationView = &animationNodeComponent->animationViews.back();
                    animationView->animationInstance = animationInstance;
                }

                animationView->channels.push_back(channel);
            }
        }
        for (int i = 0; i < armatureInstances.size(); i++)
        {
            ArmatureInstance* skinInstance = &armatureInstances[i];
            Armature* armature = scenes->armatures[i].get();
            skinInstance->armature = armature;
            skinInstance->joints.resize(armature->jointIDs.size());
            for (int j = 0; j < armature->jointIDs.size(); j++)
            {
                skinInstance->joints[j] = nodes[armature->jointIDs[j]];
            }
        }
    }
};

#endif