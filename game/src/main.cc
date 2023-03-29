#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include "Scenes.h"

int main()
{

	CDResourcesDir();

	Window window;

	MeshAttribute att;
	att.POSITION = {{1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, {-1,  1, 0}};
	MeshPrimitive prim;
	prim.attribute = att;
	prim.indices = {0, 1, 3, 1, 2, 3};
	prim.SetupGLPrimitive();
	Shader shader;
	shader.Load("shader/default_vs.glsl", "shader/default_fs.glsl");
	shader.Use();
	
	
	
	while (window.IsOpen())
	{
		window.Tick();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		prim.Draw();


		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
		
	}

	return 0;
}