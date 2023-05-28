#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Render/Camera/Camera.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Object/Actor.h"
#include "Gameplay/Object/Role.h"
#include "Render/Render/Render.h"
#include "Core/Temp/Temp.h"
#include "Core/Parse/TreeFileParse.h"
#include "Gameplay/Component/AnimationPlayerComponent.h"
#include "Gameplay/World/WorldGenerate.h"

int main()
{
	CDResourcesDir();
	Window window;
	Actor world;
	world.AddChild<Role>();	
	GenSandbox(&world);

	// Init
	world.Start();
	world.ResetWorldMatrix(true);

	while (window.IsOpen())
	{
		window.Tick();
		
		// Tick each object
		world.Tick();
		world.ResetWorldMatrix();


		// Render
		// GLFrameBuffer fbo;
		// fbo.Bind();
		// GLTexture2D colTex;
		// colTex.Bind();
		// colTex.SetupPixels(GLTexParam::RGB, window.GetSize().x, window.GetSize().y, GLTexParam::RGB, GLTexParam::Ubyte, NULL);
		// colTex.SetupFilter(GLTexParam::Linear, GLTexParam::Linear);
		// fbo.SetAttachmentTexture2D(GLFrameBufferParam::ColorAttachment0, colTex.id);
		// GLRenderBuffer rbo;
		// rbo.Bind();
		// rbo.SetStorage(GLRenderBufferParam::Depth24Stencil8, window.GetSize().x, window.GetSize().y);
		// fbo.SetRenderbuffer(GLFrameBufferParam::DepthStencilAttachment, rbo.id);
		// if (!fbo.Check())
		// {
		// 	LOG("framebuffer is not complete")
		// }
		Render render;
		render.Load(&world);
		render.Draw();
	
		// Others
		if (window.keys[KEY::ESCAPE].pressDown)
		{
			window.Close();
		}
		if (window.keys[KEY::F8].pressDown)
		{
			int mode = glfwGetInputMode(window.glfw_window, GLFW_CURSOR);
			if (mode == GLFW_CURSOR_DISABLED)
			{
				glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
	return 0;
}