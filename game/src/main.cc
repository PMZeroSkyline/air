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


int main()
{
	CDResourcesDir();
	Window window;

	// test beg


	// end
	Map map;
	Role* roleActor = map.AddChild<Role>();
	roleActor->Load("idle/idle.gltf");

	Actor* planeActor = map.AddChild<Actor>();
	ScenesComponent* planeScenesComponent = planeActor->AddComponent<ScenesComponent>();
	planeScenesComponent->Load("base/checkerPlane.gltf");

	map.ResetWorldMatrix();
	map.Start();

	Renderables renderables;
	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		map.Tick();
		renderables.Load(&map);
		renderables.Render();
		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}