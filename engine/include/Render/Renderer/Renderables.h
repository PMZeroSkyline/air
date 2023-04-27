#ifndef RENDERABLES_H
#define RENDERABLES_H

#include "Renderable.h"
#include "Gameplay/Component/CameraComponent.h"

class Renderables
{
public:
	CameraComponent* cameraComponent = nullptr;
	vector<shared_ptr<Renderable>> renderables;
	void Load(Actor* root)
	{
		renderables.clear();
		RForEachNode<Actor>(root, [this](Actor* current){
			CameraComponent* cc = current->GetComponent<CameraComponent>();
			if (cc)
			{
				cameraComponent = cc;
			}
			MeshComponent* mc = current->GetComponent<MeshComponent>();
			if (mc)
			{
				SkinComponent* sc = current->GetComponent<SkinComponent>();
				for (int i = 0; i < mc->mesh->primitives.size(); i++)
				{
					shared_ptr<Renderable> renderable = make_shared<Renderable>();
					renderable->primitive = mc->mesh->primitives[i];
					renderable->meshComponent = mc;
					renderable->skinComponent = sc;
					renderables.push_back(renderable);
				}
			}
		});
	}
	void Render()
	{
		mat4 V, P;
		if (cameraComponent)
		{
			V = RightHandUpZToUpYProjection() * ((Actor*)cameraComponent->owner)->worldMatrix.inverse();
			P = cameraComponent->camera.perspective.GetPerspectiveMatrix();
		}
		else
		{
			V = mat4().translate(vec3(0,0,0));
			P = Camera().perspective.GetPerspectiveMatrix();
		}
		for (int i = 0; i < renderables.size(); i++)
		{
			shared_ptr<Renderable> renderable = renderables[i];
			mat4 M = ((Actor*)renderable->meshComponent->owner)->worldMatrix;
			renderable->primitive->material->mat4PtrMap["M"] = &M;
			renderable->primitive->material->mat4PtrMap["V"] = &V;
			renderable->primitive->material->mat4PtrMap["P"] = &P;
			if (renderable->skinComponent)
			{
				mat4 IW = renderable->meshComponent->owner->parent ? ((Actor*)renderable->meshComponent->owner->parent)->worldMatrix.inverse() : mat4();
				renderable->primitive->material->jointMatrix.resize(128);
				for (int j = 0; j < renderable->skinComponent->skinInstance->joints.size(); j++)
				{
					renderable->primitive->material->jointMatrix[j] = IW * renderable->skinComponent->skinInstance->joints[j]->worldMatrix * renderable->skinComponent->skinInstance->skin->inverseBindMatrices[j];
				}
			}
			renderable->primitive->Draw();
		}
	}
};


#endif