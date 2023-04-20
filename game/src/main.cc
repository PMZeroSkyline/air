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


	
	
	while (window.IsOpen())
	{
		window.Tick();
		
		glEnable(GL_DEPTH_TEST);  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

		
		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
		
	}
	
	return 0;
}