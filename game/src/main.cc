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
	att.POSITION = {{0,0,0}, {1,0,0}, {0,0,1}};
	MeshPrimitive prim;
	prim.attribute = att;
	prim.indices = {0, 1, 2};
	prim.SetupGLPrimitive();
	Shader shader;
	shader.Load("shader/default_vs.glsl", "shader/default_fs.glsl");
	shader.Use();
	prim.Draw();
	
	
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