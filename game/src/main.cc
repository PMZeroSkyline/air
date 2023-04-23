#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Renderer/Camera/Camera.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Map/Map.h"
#include "Gameplay/Object/Actor.h"


class Renderable
{
public:
	shared_ptr<MeshPrimitive> primitive;
	MeshComponent* meshComponent;
	SkinComponent* skinComponent;
    bool operator<(const Renderable& rhs) const
    {
        return *primitive < *(rhs.primitive);
    }
};
class Renderables
{
public:
	CameraComponent* cameraComponent = nullptr;
	vector<shared_ptr<Renderable>> renderables;
	void Load(Actor* root)
	{
		InvForEachNode<Actor>(root, [this](Actor* current){
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
			V = ((Actor*)cameraComponent->owner)->worldMatrix.inverse();
			P = cameraComponent->camera.perspective.GetPerspectiveMatrix();
		}
		else
		{
			V = mat4().translate(vec3(0,0,-5));
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

int main()
{
	CDResourcesDir();
	Window window;

	Map map;
	Actor* sa = map.AddChild<Actor>();
	ScenesComponent* sc = sa->AddComponent<ScenesComponent>();
	sc->Load("vroid/anim/anim.gltf");
	sc->animationInstances[0].weight = 1.f;
	
	Actor* ca = map.AddChild<Actor>();
	ca->localTransform.translation.y = 1.5;
	ca->localTransform.translation.z = 1;
	ca->localTransform.rotation = EulerToQuat(vec3(-15,0,0));
	ca->AddComponent<CameraComponent>();
	// InvForEachNode<Actor>(sa, [](Actor* curr){
	// 	Actor* tmp = curr->AddChild<Actor>();
	// 	tmp->localTransform.scaling = vec3(8.f);
	// 	MeshComponent* tmp1 = tmp->AddComponent<MeshComponent>();
	// 	tmp1->mesh = make_shared<Mesh>();
	// 	shared_ptr<MeshPrimitive> tmp2 = MakeCubePrimitive();
	// 	tmp1->mesh->primitives.push_back(tmp2);
	// });
	
	Renderables renderables;
	

	while (window.IsOpen())
	{
		window.Tick();
		
		GLClear();

		sc->animationInstances[0].time = (sin(window.time)*.5+.5) * sc->animationInstances[0].animation->samplersInputMax;
		map.ResetWorldMatrix();
		renderables.Load(&map);
		renderables.Render();

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}