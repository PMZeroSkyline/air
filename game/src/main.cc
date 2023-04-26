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

	Actor* a1 = map.AddChild<Actor>();
	ScenesComponent* s1 = a1->AddComponent<ScenesComponent>();
	s1->Load("bar_zup/bar.gltf");

	Actor* a2 = map.AddChild<Actor>();
	a2->localTransform.scaling *= 10.f;
	ScenesComponent* s2 = a2->AddComponent<ScenesComponent>();
	s2->Load("idle_zup/idle.gltf");
	s2->animationInstances[0].weight = 1.f;
	s2->animationInstances[0].time = 1.f;

	Renderables renderables;
	map.Start();
	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		map.Tick();
	map.ResetWorldMatrix(true);

		renderables.Load(&map);
		renderables.Render();
		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	return 0;
}