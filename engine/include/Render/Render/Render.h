#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"

class RenderPrimitive
{
public:
    mat4* parentWorldMatrix = nullptr;
    mat4* worldMatrix = nullptr;
    MeshPrimitive* meshPrimitive = nullptr;
    ArmatureInstance* armatureInstance = nullptr;
    Material* material = nullptr;
};
void CollectionRenderPrimitives(MeshComponent* meshComponent, vector<RenderPrimitive>& renderPrimitives)
{
    RenderPrimitive renderPrimitive;
    if (meshComponent->mesh)
    {
        for (shared_ptr<MeshPrimitive> meshPrimitive : meshComponent->mesh->primitives)
        {
            renderPrimitive.meshPrimitive = meshPrimitive.get();
            renderPrimitive.material = meshPrimitive->material.get();
            renderPrimitive.armatureInstance = meshComponent->armatureInstance;
            if (meshComponent->owner)
            {
                renderPrimitive.worldMatrix = &((Actor*)meshComponent->owner)->worldMatrix;
            }
            if (meshComponent->owner->parent)
            {
                renderPrimitive.parentWorldMatrix = &((Actor*)meshComponent->owner->parent)->worldMatrix;
            }
            renderPrimitives.push_back(renderPrimitive);
        }
    }
}
void CollectionRenderPrimitives(Actor* root, vector<RenderPrimitive>& renderPrimitives)
{
    vector<MeshComponent*> meshComponents = root->GetComponents<MeshComponent>(true);
    for (MeshComponent* meshComponent : meshComponents)
    {
        CollectionRenderPrimitives(meshComponent, renderPrimitives);
    }
}
vector<RenderPrimitive> CollectionRenderPrimitives(Actor* root)
{
    vector<RenderPrimitive> renderPrimitives;
    CollectionRenderPrimitives(root, renderPrimitives);
    return renderPrimitives;
}

#endif