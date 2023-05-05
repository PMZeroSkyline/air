#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Render/Camera/Camera.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Map/Map.h"
#include "Gameplay/Object/Actor.h"
#include "Gameplay/Object/Role.h"
#include "Render/Renderer/Renderables.h"
#include "Core/Temp/Temp.h"
#include "Core/Parse/TreeFileParse.h"

int main()
{
	CDResourcesDir();
	Window window;

	Map map;
	map.name = "Map";
	map.AddChild<Role>();	
	Actor* a1 = map.AddChild<Actor>();
	a1->name = "Sandbox";
	ScenesComponent* s1 = a1->AddComponent<ScenesComponent>();
	s1->Load("sandbox/sandbox.gltf");
	s1->FieldExpand();

	Actor* a2 = map.AddChild<Actor>();
	a2->name = "A2";
	ScenesComponent* s2 = a2->AddComponent<ScenesComponent>();
	s2->Load("vroid/vroid.gltf");
	s2->FieldExpand();

	Renderables renderables;
	map.Start();
	map.ResetWorldMatrix(true);
	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		
		map.Tick();
		renderables.Load(&map);
		renderables.Render();
	
	// Actor* cube = FindNodeByName<Actor>("Cube.010", a1);
	// if (cube)
	// {
	// 	MeshComponent* cCube = cube->GetComponent<MeshComponent>();
	// 	if (cCube)
	// 	{
	// 		mat4 cs = cube->worldMatrix.inverse() * roleActor->worldMatrix;
	// 		vec3 test(cs[0][3],cs[1][3],cs[2][3]);
	// 		LOG(cCube->mesh->primitives[0]->boundingBox.Check(test));
	// 	}
	// }
		
		if (window.keys[KEY::ESCAPE].pressDown)
		{
			window.Close();
		}
		if (window.keys[KEY::F8].pressDown)
		{
			int mode = glfwGetInputMode(window.glfw_window, GLFW_CURSOR);
			if (mode == GLFW_CURSOR_DISABLED)
			{
				glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
	return 0;
}