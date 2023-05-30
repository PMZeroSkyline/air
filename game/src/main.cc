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
#include "Physic/Casting/Casting.h"

int main()
{
	CDResourcesDir();
	Window window;
	Actor world;
	Role* aRole = world.AddChild<Role>();	
	GenSandbox(&world);



	Actor* aDirLight = world.AddChild<Actor>();
	aDirLight->localTransform.translation = vec3(0.f, 0.f, 10.f);
	aDirLight->localTransform.rotation = EulerToQuat(vec3(-90.f, 0.f, 0.f));
	CameraComponent* cLightCam = aDirLight->AddComponent<CameraComponent>();
	shared_ptr<OrthographicCamera> depthCamera = make_shared<OrthographicCamera>();
	depthCamera->isUseFramebufferAspect = false;
	cLightCam->camera = depthCamera;

	shared_ptr<MeshPrimitive> mpQuad = MakeQuadMeshPrimitive();
	mpQuad->material->shader = MakeShaderFromRes("screen");
	mpQuad->material->depthTest = false;


	// Render
	GLFrameBuffer depthFBO;
	depthFBO.Bind();
	GLTexture2D depthTex;
	depthTex.Bind();
	depthTex.SetupPixels(GLTexParam::DEPTH_COMPONENT, 1024, 1024, GLTexParam::DEPTH_COMPONENT, GLTexParam::FLOAT, NULL);
	//depthTex.SetupPixels(GLTexParam::DEPTH_COMPONENT, window.GetSize().x, window.GetSize().y, GLTexParam::DEPTH_COMPONENT, GLTexParam::FLOAT, NULL);
	depthTex.SetupFilters(GLTexParam::LINEAR, GLTexParam::NEAREST);
	depthTex.SetupWrapST(GLTexParam::REPEAT, GLTexParam::REPEAT);
	depthFBO.SetAttachmentTexture2D(GLFrameBufferParam::DEPTH_ATTACHMENT, depthTex.id);
	// glDrawBuffer(GL_NONE);
	// glReadBuffer(GL_NONE);
	// glViewport(0, 0, 1024, 1024);
	
	
	mpQuad->material->textureMap["screenTex"] = &depthTex;

	// Init
	world.Start();
	world.ResetWorldMatrix(true);

	while (window.IsOpen())
	{
		window.Tick();
		
		// Tick each object
		world.Tick();
		world.ResetWorldMatrix();

		depthFBO.Bind();
		GLClear();	
		Render render;
		render.Load(&world);
		render.cameraComponent = cLightCam;
		render.Draw();

		GLBindDefaultFrameBuffer();
		// render.cameraComponent = aRole->camComp;
		// for (int i = 0; i < render.renderables.size(); i++)
		// {
		// 	render.renderables[i]->primitive->material->textureMap["shadowTex"] = &depthTex;
		// 	render.renderables[i]->primitive->material->matrixMap["L"] = cLightCam->GetProjectioMatrix() * RightHandZUpToYUpProjection() * ((Actor*)cLightCam->owner)->worldMatrix.inverse();
		// }
		mpQuad->Draw();
		
	
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