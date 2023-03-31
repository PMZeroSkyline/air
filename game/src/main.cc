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
	
	shared_ptr<MeshPrimitive> quad = MakeQuad();
	shared_ptr<Image> img = make_shared<Image>();
	img->Load("idle/image.png");
	shared_ptr<Sampler> sampler = make_shared<Sampler>();
	shared_ptr<Texture2D> tex = make_shared<Texture2D>();
	tex->image = img;
	tex->sampler = sampler;
	tex->SetupGLTexture2D();
	quad->material->texturePairs.push_back(make_pair("tex", tex));
	quad->material->shader->Use();
	quad->material->Setup();
	
	

	while (window.IsOpen())
	{
		window.Tick();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		quad->material->Bind();
		quad->Draw();
		

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}

	return 0;
}