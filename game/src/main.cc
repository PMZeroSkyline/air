#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Renderer/Camera/Camera.h"
#include "Gameplay/Component/CameraComponent.h"

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
	}
	void Render()
	{
		mat4 V = ((Actor*)cameraComponent->owner)->worldMatrix.inverse();
		mat4 P = cameraComponent->camera.perspective.GetPerspectiveMatrix();

		sort(renderables.begin(), renderables.end());

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

	Actor* root = new Actor;

	Actor* ca = root->AddChild<Actor>();
	CameraComponent* c = ca->AddComponent<CameraComponent>();
	ca->localTransform.translation = vec3(0,5,5);
	ca->localTransform.rotation = EulerToQuat(vec3(-45, 0, 0));
	ca->worldMatrix = ca->localTransform.ToMatrix();

	Actor* sa = root->AddChild<Actor>();
	sa->localTransform.rotation = EulerToQuat(vec3(0, 0, 0));
	ScenesComponent* sc = sa->AddComponent<ScenesComponent>();
	sc->Load("sushi_bar/scene.gltf");
	sc->FieldExpand();

	Renderables renderables;
	renderables.Load(root);

	while (window.IsOpen())
	{
		window.Tick();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		GLClear();
		//sc->animationInstances[0].weight = 1.f;
		//sc->animationInstances[0].time = (sin(window.time) * 0.5 + 0.5) * 2;
		ResetWorldMatrix(root);
		renderables.Render();
		//RenderActor(sa, c);

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	delete sa;
	delete ca;
	return 0;
}