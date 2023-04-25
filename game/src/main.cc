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

	// vec3 e0(-30.f, 100.f, 0.1f);
	
	// quat q = EulerToQuat(e0);
	// vec3 e1 = QuatToEuler(q);
	// LOG("")
	

	// end
	Map map;
	Actor* a1 = map.AddChild<Actor>();
	ScenesComponent* s1 = a1->AddComponent<ScenesComponent>();
	s1->Load("bar/scene.gltf");

	Actor* a2 = map.AddChild<Actor>();
	a2->localTransform.translation = vec3(0,-5,5);
	a2->localTransform.rotation = EulerToQuat(45.f, 0.f, 0.f);
	CameraComponent* c1 = a2->AddComponent<CameraComponent>();

	// Role* roleActor = map.AddChild<Role>();
	// roleActor->Load("idle/idle.gltf");

	// Actor* planeActor = map.AddChild<Actor>();
	// ScenesComponent* planeScenesComponent = planeActor->AddComponent<ScenesComponent>();
	// planeScenesComponent->Load("base/checkerPlane.gltf");

	map.ResetWorldMatrix();

	Renderables renderables;
	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		renderables.Load(&map);
		renderables.Render();
		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}