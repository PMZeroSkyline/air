#ifndef SCENES_COMPONENT_H
#define SCENES_COMPONENT_H

#include "Component.h"
#include "Resource/Model/Scenes.h"
#include "Gameplay/Object/Actor.h"
#include "Animation/AnimationState/AnimationState.h"
#include "Animation/AnimationState/AnimationView.h"
#include "AnimationNodeComponent.h"

class ScenesComponent : public Component
{
public:
    shared_ptr<Scenes> scenes;
    AnimationState animationState;

    vector<Actor*> nodes;

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
        Scene* scene = &scenes->scenes[scenes->sceneID];
        name = scene->name;
        for (int i = 0; i < scene->nodeIDs.size(); i++)
        {
            SceneNode* sceneNode = &scenes->nodes[scene->nodeIDs[i]];
            Actor* nodeChild = owner->AddChild<Actor>();
            nodes[scene->nodeIDs[i]] = nodeChild;
            NodeExpand(sceneNode, nodeChild);
        }

        animationState.animationInstances.resize(scenes->animations.size());
        for (int i = 0; i < scenes->animations.size(); i++)
        {
            shared_ptr<Animation> animation = scenes->animations[i];
            animationState.animationInstances[i].animation = animation.get();
            animationState.animationInstances[i].weight = 0.f;

            for (int j = 0; j < animation->channels.size(); j++)
            {
                AnimationChannel* channel = &animation->channels[j];
                Actor* targetNode = nodes[channel->target.nodeID];
                AnimationNodeComponent* animationNodeComponent = targetNode->GetComponent<AnimationNodeComponent>();
                if (!animationNodeComponent)
                {
                    animationNodeComponent = targetNode->AddComponent<AnimationNodeComponent>();
                }
                AnimationView* animationView = nullptr;
                for (int k = 0; k < animationNodeComponent->animationViews.size(); k++)
                {
                    if (animationNodeComponent->animationViews[k].animationInstance == &animationState.animationInstances[i]);
                    {
                        animationView = &animationNodeComponent->animationViews[k];
                        break;
                    }
                }
                if (!animationView)
                {
                    animationNodeComponent->animationViews.push_back(AnimationView());
                    animationView = &animationNodeComponent->animationViews.back();
                    animationView->animationInstance = &animationState.animationInstances[i];
                }
                animationView->channels.push_back(channel);
                
            }
            
            
            
        }
    }
};

#endif