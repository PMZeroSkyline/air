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

int main()
{
	CDResourcesDir();
	Window window;

	// Make Root
	Actor map;
	map.name = "Map";

	// Add Player
	map.AddChild<Role>();	

	// Make floor
	Actor* a1 = map.AddChild<Actor>();
	MeshComponent* mc = a1->AddComponent<MeshComponent>();
	a1->localTransform.scaling = vec3(10.f);
	mc->mesh = make_shared<Mesh>();
	mc->mesh->primitives.push_back(MakeQuadMeshPrimitive());
	// ScenesComponent* sc = a1->AddComponent<ScenesComponent>();
	// sc->Load("untitled.gltf");
	// sc->FieldExpand();

	// Add floor rotation
	//vector<AnimationInstance> animInsts;
	//animInsts.resize(1);
	//AnimationInstance& animInst = animInsts[0];
	//Animation anim;
	//anim.name = "Test";
	//anim.samplers.resize(1);
	//anim.channels.resize(1);
	//AnimationSampler& animSamp = anim.samplers[0];
	//animSamp.outputQuats.resize(2);
	//animSamp.outputQuats[0] = EulerToQuat(vec3(0.f,0.f,30.f));
	//animSamp.outputQuats[1] = EulerToQuat(vec3(0.f,0.f,90.f));
	//animSamp.inputs.resize(2);
	//animSamp.inputs[0] = 0.f;
	//animSamp.inputs[1] = 1.f;
	//animSamp.max = 1.f;
	//anim.ResetMinMax();
	//AnimationChannel& animChanel = anim.channels[0];
	//animChanel.sampler = &animSamp;
	//animChanel.target.path = AnimationChannelTargetPath::rotation;
	//animInst.animation = &anim;
	//AnimationNodeComponent* an = a1->AddComponent<AnimationNodeComponent>();
	//AnimationView av;
	//av.animationInstance = &animInst;
	//av.channels.push_back(&animChanel);
	//an->animationViews.push_back(av);
	//AnimationPlayerComponent* animPlay = a1->AddComponent<AnimationPlayerComponent>();
	//animPlay->animInsts = &animInsts;
	//animPlay->Play("Test", true);

	// Init
	map.Start();
	map.ResetWorldMatrix(true);

	while (window.IsOpen())
	{
		window.Tick();
		GLClear();
		
		// Tick each object
		map.Tick();
		map.ResetWorldMatrix();


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
		render.Load(&map);
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