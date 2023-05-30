#ifndef RENDERER_H
#define RENDERER_H

#include "Renderable.h"
#include "Gameplay/Component/CameraComponent.h"

class RenderableView
{
public:
	vector<shared_ptr<RenderablePrimitive>> renderables;

	void Load(Actor* root)
	{
		renderables.clear();
		root->RForEachNode<Actor>([this](Actor* current){
			vector<MeshComponent*> mcs = current->GetComponents<MeshComponent>();
			for (int i = 0; i < mcs.size(); i++)
			{
				MeshComponent* mc = mcs[i];
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
			}
		});
		sort(renderables.begin(), renderables.end(), [](const shared_ptr<Renderable>& lhs, const shared_ptr<Renderable>& rhs){
			return lhs->primitive->material->alphaMode < rhs->primitive->material->alphaMode;
		});
	}
};

class RenderablesView
{
public:
	CameraComponent* cameraComponent = nullptr;
	vector<shared_ptr<Renderable>> renderables;

	void Load(Actor* root)
	{
		renderables.clear();
		root->RForEachNode<Actor>([this](Actor* current){
			CameraComponent* cc = current->GetComponent<CameraComponent>();
			if (!cameraComponent && cc)
			{
				cameraComponent = cc;
			}
			vector<MeshComponent*> mcs = current->GetComponents<MeshComponent>();
			for (int i = 0; i < mcs.size(); i++)
			{
				MeshComponent* mc = mcs[i];
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
			}
		});
		sort(renderables.begin(), renderables.end(), [](const shared_ptr<Renderable>& lhs, const shared_ptr<Renderable>& rhs){
			return lhs->primitive->material->alphaMode < rhs->primitive->material->alphaMode;
		});
	}
	void Draw()
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
			shared_ptr<Renderable> renderable = renderables[i];
			mat4 M = ((Actor*)renderable->meshComponent->owner)->worldMatrix;
			renderable->primitive->material->matrixMap["M"] = M;
			renderable->primitive->material->matrixMap["V"] = V;
			renderable->primitive->material->matrixMap["P"] = P;
			if (renderable->skinComponent)
			{
				mat4 IW = renderable->meshComponent->owner->parent ? ((Actor*)renderable->meshComponent->owner->parent)->worldMatrix.inverse() : mat4();
				vector<mat4> jointMatrices;
				jointMatrices.resize(128);
				for (int j = 0; j < renderable->skinComponent->skinInstance->joints.size(); j++)
				{
					jointMatrices[j] = IW * renderable->skinComponent->skinInstance->joints[j]->worldMatrix * renderable->skinComponent->skinInstance->skin->inverseBindMatrices[j];
				}
				renderable->primitive->material->matricesMap["J"] = move(jointMatrices);
			}
			renderable->primitive->Draw();
		}
	}
};

#endif