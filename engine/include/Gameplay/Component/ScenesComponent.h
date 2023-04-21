#ifndef SCENES_COMPONENT_H
#define SCENES_COMPONENT_H

#include "Component.h"
#include "Resource/Model/Scenes.h"
#include "Gameplay/Object/Actor.h"
#include "Animation/AnimationState/AnimationInstanceView.h"
#include "AnimationNodeComponent.h"
#include "MeshComponent.h"
#include "SkinComponent.h"

class ScenesComponent : public Component
{
public:
    shared_ptr<Scenes> scenes;
    vector<AnimationInstance> animationInstances;

    vector<Actor*> nodes;
    vector<SkinInstance*> skinInstances;

    void Load(const string& path)
    {
        scenes = scenesBlob.Get(path);
        if (!scenes)
        {
            scenes = make_shared<Scenes>();
            scenes->Load(path);
            scenesBlob.Set(path, scenes);
        }
    }

    void NodeExpand(SceneNode* sceneNode, Actor* node)
    {           
        node->name = sceneNode->name;
        node->localTransform = sceneNode->localTransform;

        if (sceneNode->meshID != -1)
        {
            MeshComponent* meshComponent = node->AddComponent<MeshComponent>();
            meshComponent->mesh = scenes->meshs[sceneNode->meshID];
        }
        if (sceneNode->skinID != -1)
        {
            SkinComponent* skinComponent = node->AddComponent<SkinComponent>();
            skinComponent->skinInstance.skin = scenes->skins[sceneNode->skinID].get();
            skinInstances[sceneNode->skinID] = &skinComponent->skinInstance;
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
        skinInstances.resize(scenes->skins.size());
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

                AnimationInstanceView* animationInstanceView = animationNodeComponent->GetAnimationInstanceView(animationInstance);
                if (!animationInstanceView)
                {
                    animationNodeComponent->animationInstanceViews.push_back(AnimationInstanceView());
                    animationInstanceView = &animationNodeComponent->animationInstanceViews.back();
                    animationInstanceView->animationInstance = animationInstance;
                }

                animationInstanceView->channels.push_back(channel);
            }
        }

        for (int i = 0; i < skinInstances.size(); i++)
        {
            SkinInstance* skinInstance = skinInstances[i];
            Skin* skin = skinInstance->skin;
            skinInstance->joints.resize(skin->jointIDs.size());
            for (int j = 0; j < skin->jointIDs.size(); j++)
            {
                skinInstance->joints[j] = nodes[skin->jointIDs[j]];
            }
        }
    }
};

#endif