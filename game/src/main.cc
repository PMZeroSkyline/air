#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scene.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Renderer/Camera/Camera.h"
int main()
{
	Window window;
	SetupOpenGL();
	CDResourcesDir();

	
	Scene scene;
	// scene.Load("vroid/vroid.gltf");
	// mat4 M, V = mat4().translate(vec3(0,-1,-5)), P = Camera().perspective.GetPerspectiveMatrix();
	// for (int i = 0; i < scene.materials.size(); i++)
	// {
	// 	scene.materials[i]->mat4PtrMap["M"] = &M;
	// 	scene.materials[i]->mat4PtrMap["V"] = &V;
	// 	scene.materials[i]->mat4PtrMap["P"] = &P;
	// }
	// CubePrimitive();
	
	while (window.IsOpen())
	{
		window.Tick();
		
		glEnable(GL_DEPTH_TEST);  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

		// float rot = mod(window.time*30.f,360.f);
		// M = mat4(quat(radians(vec3(rot, 0, 0))));

		// for (int i = 0; i < scene.meshs.size(); i++)
		// {
		// 	scene.meshs[i]->primitives[0].Draw();
		// }
		

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
		
	}
	
	return 0;
}