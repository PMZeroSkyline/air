#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Renderer/Camera/Camera.h"

int main()
{
	CDResourcesDir();
	Window window;

	Actor* a = new Actor;
	ScenesComponent* scenes = a->AddComponent<ScenesComponent>();
	scenes->Load("idle/idle.gltf");
	scenes->FieldExpand();
	LOG(" ");
	
	mat4 M, V = mat4().translate(vec3(0,0,-5.f)), P = Camera().perspective.GetPerspectiveMatrix();


	for (int i = 0; i < scenes->scenes->materials.size(); i++)
	{
		scenes->scenes->materials[i]->mat4PtrMap["M"] = &M;
		scenes->scenes->materials[i]->mat4PtrMap["V"] = &V;
		scenes->scenes->materials[i]->mat4PtrMap["P"] = &P;
	}
	

	while (window.IsOpen())
	{
		window.Tick();
		
		GLClear();

		for	(int i = 0; i != scenes->scenes->meshs.size(); i++)
		{
			for (int j = 0; j < scenes->scenes->meshs[i]->primitives.size(); j++)
			{
				scenes->scenes->meshs[i]->primitives[j].Draw();
			}
		}
		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	
	return 0;
}