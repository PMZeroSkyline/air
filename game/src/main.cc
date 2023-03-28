#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include "Scenes.h"

int main()
{

	CDResourcesDir();

	Window window;

	Actor model;
	ScenesComponent* scenesComponent = model.AddComponent<ScenesComponent>();
	scenesComponent->Load("idle/idle.gltf");
	scenesComponent->FieldsExpand();
	cout << " " << endl;
	
	while (window.IsOpen())
	{
		window.Tick();

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
		
	}

	return 0;
}