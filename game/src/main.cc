#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"
#include "Scene.h"
#include "Renderer.h"



int main()
{
	Window window;
	SetupOpenGL();
	CDResourcesDir();
	
	Scene scene;
	scene.Load("sushi_bar/scene.gltf");
	Material material;
	material.UseDefaultShader();
	material.texturePairs.push_back(make_pair("baseColorTex", make_shared<Texture2D>()));
	material.texturePairs[0].second->image = make_shared<Image>();
	material.texturePairs[0].second->image->Load("idle/maria_diffuse.png");
	material.texturePairs[0].second->SetupGLTexture2D();
	material.Setup();
	material.shader->Use();
	scene.meshs[0]->primitives[0].SetupGLPrimitive();

	mat4 M(1.f), V(1.f), P(1.f);
	P = Camera().perspective.GetPerspectiveMatrix();
	M = translate(M, vec3(0,-1,-5.f));
	material.shader->SetMat4("M", M);
	material.shader->SetMat4("V", V);
	material.shader->SetMat4("P", P);

	
	while (window.IsOpen())
	{
		window.Tick();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		material.Bind();
		scene.meshs[0]->primitives[0].Draw();


		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}

	return 0;
}