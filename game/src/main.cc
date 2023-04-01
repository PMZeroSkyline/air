#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include "Scenes.h"
#include "Renderer.h"

int main()
{
	Window window;
	SetupOpenGL();
	CDResourcesDir();
	
	Scenes scenes;
	scenes.Load("test/1.fbx");
	//scenes.materials[0]->shader->Use();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	while (window.IsOpen())
	{
		window.Tick();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		//scenes.materials[0]->Bind();
		//scenes.meshs[0]->primitives[0].Draw();
		

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}

	return 0;
}