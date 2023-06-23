#ifndef SCENES_COMPONENT_H
#define SCENES_COMPONENT_H

#include "Gameplay/Object/Actor.h"
#include "Component.h"

#include "Resource/Model/Scenes.h"

#include "MeshComponent.h"

#include "SkinComponent.h"
#include "Animation/SkinInstance.h"

#include "Animation/AnimationView.h"
#include "AnimationNodeComponent.h"

class ScenesComponent : public Component
{
public:
    shared_ptr<Scenes> scenes;
    vector<Actor*> nodes;
    vector<SkinInstance> skinInstances;
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

        if (sceneNode->meshID != -1)
        {
            MeshComponent* meshComponent = node->AddComponent<MeshComponent>();
            meshComponent->mesh = scenes->meshs[sceneNode->meshID];
        }
        if (sceneNode->skinID != -1)
        {
            SkinComponent* skinComponent = node->AddComponent<SkinComponent>();
            skinComponent->skinInstance = &skinInstances[sceneNode->skinID];
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
                
                // vector<Actor*>::iterator found = find_if(nodes.begin(), nodes.end(), [&channel](Actor* node){
                //     return channel->target.name == node->name;
                // });
                // if (found == nodes.end())
                // {
                //     LOG("cann't found animation target " << channel->target.name << " !")
                //     continue;
                // }
                // Actor* targetNode = *found;

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
        for (int i = 0; i < skinInstances.size(); i++)
        {
            SkinInstance* skinInstance = &skinInstances[i];
            Skin* skin = scenes->skins[i].get();
            skinInstance->skin = skin;
            skinInstance->joints.resize(skin->jointIDs.size());
            for (int j = 0; j < skin->jointIDs.size(); j++)
            {
                skinInstance->joints[j] = nodes[skin->jointIDs[j]];
            }
            // skinInstance->joints.resize(skin->jointNames.size());
            // for (int j = 0; j < skin->jointNames.size(); j++)
            // {
            //     string jointName = skin->jointNames[j];
            //     auto found = find_if(nodes.begin(), nodes.end(), [&jointName](Actor* node){
            //         return jointName == node->name;
            //     });
            //     if (found == nodes.end())
            //     {
            //         LOG("not found skin joint")
            //         continue;
            //     }
            //     skinInstance->joints[j] = *found;
            // }
        }
    }
};

#endif