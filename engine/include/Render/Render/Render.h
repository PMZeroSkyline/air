#ifndef RENDER_H
#define RENDER_H

#include "Renderable.h"
#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/CameraComponent.h"

class Render
{
public:
    vector<shared_ptr<RenderablePrimitive>> renderables;
    virtual void Load(Actor* root) = 0;
    virtual void Draw(shared_ptr<Material> material = make_shared<Material>()) = 0;
};
// class Render2D : public Render
// {

// };
class WorldRender : public Render
{
public:
    CameraComponent* cameraComponent;
    virtual void Load(Actor* root) override
    {
        renderables.clear();
        root->RForEachNode<Actor>([this](Actor* actor){
            vector<MeshComponent*> mcs = actor->GetComponents<MeshComponent>();
            SkinComponent* sc = actor->GetComponent<SkinComponent>();
			for (int i = 0; i < mcs.size(); i++)
			{
				MeshComponent* mc = mcs[i];
				for (int i = 0; i < mc->mesh->primitives.size(); i++)
				{
                    if (sc)
                    {
                        shared_ptr<RenderableSkin> renderableSkin = make_shared<RenderableSkin>();
                        renderableSkin->meshComponent = mc;
                        renderableSkin->skinComponent = sc;
                        renderableSkin->primitive = mc->mesh->primitives[i];
                        renderables.push_back(renderableSkin);
                    }
                    else
                    {
                        shared_ptr<RenderableMesh> renderableMesh = make_shared<RenderableMesh>();
                        renderableMesh->meshComponent = mc;
                        renderableMesh->primitive = mc->mesh->primitives[i];
                        renderables.push_back(renderableMesh);
                    }
				}
			}        
		});
        sort(renderables.begin(), renderables.end(), [](const shared_ptr<RenderablePrimitive>& lhs, const shared_ptr<RenderablePrimitive>& rhs){
			return lhs->primitive->material->alphaMode < rhs->primitive->material->alphaMode;
		});
    }
    virtual void Draw(shared_ptr<Material> overrideMaterial = make_shared<Material>()) override
    {
        mat4 V, P;
		if (cameraComponent)
		{
			V = RightHandZUpToYUpProjection() * ((Actor*)cameraComponent->owner)->worldMatrix.inverse();
			P = cameraComponent->camera->GetProjectioMatrix();
		}
		else
		{
			V = RightHandZUpToYUpProjection();
			P = PerspectiveCamera().GetProjectioMatrix();
		}
		for (int i = 0; i < renderables.size(); i++)
		{
            shared_ptr<RenderablePrimitive> renderable = renderables[i];
            RenderableMesh* renderableMesh = (RenderableMesh*)renderable.get();
            mat4 M = ((Actor*)renderableMesh->meshComponent->owner)->worldMatrix;
			renderable->primitive->material->matrixMap["M"] = M;
			renderable->primitive->material->matrixMap["V"] = V;
			renderable->primitive->material->matrixMap["P"] = P;
            if(IsSharedType(renderable, RenderableSkin))
            {
                RenderableSkin* renderableSkin = (RenderableSkin*)renderable.get();
				mat4 IW = renderableSkin->meshComponent->owner->parent ? ((Actor*)renderableSkin->meshComponent->owner->parent)->worldMatrix.inverse() : mat4();
				vector<mat4> jointMatrices;
				jointMatrices.resize(128);
				for (int j = 0; j < renderableSkin->skinComponent->skinInstance->joints.size(); j++)
				{
					jointMatrices[j] = IW * renderableSkin->skinComponent->skinInstance->joints[j]->worldMatrix * renderableSkin->skinComponent->skinInstance->skin->inverseBindMatrices[j];
				}
				renderableSkin->primitive->material->matricesMap["J"] = move(jointMatrices);
            }
			renderableMesh->primitive->Draw(overrideMaterial);
		}
    }
};

#endif