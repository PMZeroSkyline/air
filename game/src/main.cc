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

	//test beg

	mat4 M = {
		{1, 0, 0, 0},
		{0, 0, -1, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 1}
	};
	Transform T = Transform(M);
	LOG(T)
	LOG("")
	

	// end
	Map map;
	// Actor* a2 = map.AddChild<Actor>();
	// a2->localTransform.translation = vec3(0,5,5);
	// a2->localTransform.rotation = EulerToQuat(-45.f, 0.f, 0.f);
	// CameraComponent* c1 = a2->AddComponent<CameraComponent>();

	Role* roleActor = map.AddChild<Role>();
	// roleActor->Load("idle/idle.gltf");

	Actor* planeActor = map.AddChild<Actor>();
	ScenesComponent* planeScenesComponent = planeActor->AddComponent<ScenesComponent>();
	planeScenesComponent->Load("bar_zup/bar.gltf");

	Renderables renderables;
	map.Start();
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