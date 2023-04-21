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
		ForEachNode(root, [this](Node* current){

			Actor* actor = (Actor*)current;
			
			if (CameraComponent* cameraComp = actor->GetComponent<CameraComponent>())
			{
				cameraComponent = cameraComp;
			}
			
			if (MeshComponent* meshComponent = actor->GetComponent<MeshComponent>())
			{
				SkinComponent* skinComponent = actor->GetComponent<SkinComponent>();
				for (int i = 0; i < meshComponent->mesh->primitives.size(); i++)
				{
					Renderable renderable;
					renderable.actor = actor;
					renderable.meshPrimitive = &meshComponent->mesh->primitives[i];
					renderable.skinComponent = skinComponent;
					renderables.push_back(renderable);
				}
			}
			return true;
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

			mat4 M = renderable->actor->worldMatrix;

			material->mat4PtrMap["M"] = &M;
			material->mat4PtrMap["V"] = &V;
			material->mat4PtrMap["P"] = &P;

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
	ScenesComponent* CBar = ABar->AddComponent<ScenesComponent>();
	CBar->Load("bar/scene.gltf");
	CBar->FieldExpand();

	Actor* ACam = map.AddChild<Actor>();
	ACam->localTransform.translation = vec3(0, 5, 5);
	ACam->localTransform.rotation = EulerToQuat(vec3(-45, 0, 0));
	CameraComponent* CCam = ACam->AddComponent<CameraComponent>();

	Renderables renderables;

	map.Start();

	while (window.IsOpen())
	{
		window.Tick();
		GLClear();

		map.Tick(window.deltaTime);
		//renderables.Load(&map);
		
		//renderables.Render();

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}