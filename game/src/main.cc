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
	Actor* actor = nullptr;
	SkinComponent* skinComponent = nullptr;
	MeshPrimitive* meshPrimitive = nullptr;
    bool operator<(const Renderable& rhs) const
    {
        return *meshPrimitive < *(rhs.meshPrimitive);
    }
};
class Renderables
{
public:
	CameraComponent* cameraComponent;
	vector<Renderable> renderables;
	void Load(Actor* root)
	{
		renderables.clear();
		InvForEachNode<Actor>(root, [this](Actor* current){
			if (CameraComponent* cameraComp = current->GetComponent<CameraComponent>())
			{
				cameraComponent = cameraComp;
			}
			
			if (MeshComponent* meshComponent = current->GetComponent<MeshComponent>())
			{
				SkinComponent* skinComponent = current->GetComponent<SkinComponent>();
				for (int i = 0; i < meshComponent->mesh->primitives.size(); i++)
				{
					Renderable renderable;
					renderable.actor = current;
					renderable.meshPrimitive = &meshComponent->mesh->primitives[i];
					renderable.skinComponent = skinComponent;
					renderables.push_back(renderable);
				}
			}
		});
		sort(renderables.begin(), renderables.end());
	}
	void Render()
	{
		mat4 V = ((Actor*)cameraComponent->owner)->worldMatrix.inverse();
		mat4 P = cameraComponent->camera.perspective.GetPerspectiveMatrix();

		for (int i = 0; i < renderables.size(); i++)
		{
			Renderable* renderable = &renderables[i];
			Material* material = renderable->meshPrimitive->material.get();
			MeshPrimitive* meshPrimitive = renderable->meshPrimitive;
			SkinComponent* skinComponent = renderable->skinComponent;
			Actor* actor = renderable->actor;
			Actor* parent = (Actor*)renderable->actor->parent;

			mat4 M = actor->worldMatrix;
			mat4 W = parent ? parent->worldMatrix : mat4();

			material->mat4PtrMap["M"] = &M;
			material->mat4PtrMap["V"] = &V;
			material->mat4PtrMap["P"] = &P;

			if (skinComponent)
			{
				SkinInstance* skinInstance = &skinComponent->skinInstance;
				Skin* skin = skinInstance->skin;
				material->jointMatrix.resize(100);
				for (int j = 0; j < skin->inverseBindMatrices.size(); j++)
				{
					material->jointMatrix[j] = skin->inverseBindMatrices[j];
				}
			}
			
			material->Bind();
			meshPrimitive->Draw();
		}		
	}
};

int main()
{
	CDResourcesDir();
	Window window;

	Map map;

	
	Actor* ABar = map.AddChild<Actor>();
	ABar->localTransform.scaling = vec3(1.f);
	ABar->localTransform.rotation = EulerToQuat(vec3(-0,0,0));
	ScenesComponent* CBar = ABar->AddComponent<ScenesComponent>();
	CBar->Load("idle/idle.gltf");
	CBar->FieldExpand();
	
	CBar->animationInstances[0].weight = 0.f;

	Actor* ACam = map.AddChild<Actor>();
	ACam->localTransform.translation = vec3(0, 0, 5);
	ACam->localTransform.rotation = EulerToQuat(vec3(0, 0, 0));
	CameraComponent* CCam = ACam->AddComponent<CameraComponent>();

	Renderables renderables;

	map.Start();
	

	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		
		CBar->animationInstances[0].time = (sin(window.time)*0.5+0.5) * CBar->animationInstances[0].animation->samplersInputMax;
		map.Tick(window.deltaTime);
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