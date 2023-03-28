#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include "Scenes.h"

int main()
{

	CDResourcesDir();

	Window window;

	Actor ac;
	Scenes scenes;
	scenes.Load("idle/idle.gltf");
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