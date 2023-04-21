#include <iostream>
#include "OS/Window/Window.h"
#include "Resource/Model/Scenes.h"
#include "Platform/File/FileDirectory.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Resource/Model/Mesh.h"
#include "Gameplay/Component/ScenesComponent.h"
#include "Renderer/Camera/Camera.h"
#include "Gameplay/Component/CameraComponent.h"


void RenderActor(Actor* node, CameraComponent* cameraComponent)
{
	SkinComponent* skinComponent = node->GetComponent<SkinComponent>();
	MeshComponent* meshComponent = node->GetComponent<MeshComponent>();
	if (meshComponent)
	{
		for (int i = 0; i < meshComponent->mesh->primitives.size(); i++)
		{
			MeshPrimitive* primitive = &meshComponent->mesh->primitives[i];
			Material* material = primitive->material.get();
			material->mat4Map["M"] = ((Actor*)node)->worldMatrix;
			material->mat4Map["V"] = ((Actor*)cameraComponent->owner)->worldMatrix.inverse();
			material->mat4Map["P"] = cameraComponent->camera.perspective.GetPerspectiveMatrix();
			
			if (skinComponent)
			{
				SkinInstance* skinInstance = &skinComponent->skinInstance;
				Skin* skin = skinInstance->skin;
				material->jointMatrix.resize(100);
				for (int j = 0; j < skinInstance->joints.size(); j++)
				{
					Actor* joint = skinInstance->joints[j];
					material->jointMatrix[j] = ((Actor*)node)->worldMatrix.inverse() * joint->worldMatrix * skin->inverseBindMatrices[j];
				}
			}

			material->Bind();
			primitive->Draw();
		}
	}
	for (int i = 0; i < node->children.size(); i++)
	{
		RenderActor((Actor*)node->children[i], cameraComponent);
	}
}

int main()
{
	CDResourcesDir();
	Window window;

	Actor* ca = new Actor;
	CameraComponent* c = ca->AddComponent<CameraComponent>();
	ca->localTransform.translation = vec3(0,2,2);
	ca->localTransform.rotation = EulerToQuat(vec3(-45, 0, 0));
	ca->worldMatrix = ca->localTransform.ToMatrix();

	Actor* sa = new Actor;
	sa->localTransform.rotation = EulerToQuat(vec3(0, 0, 0));
	ScenesComponent* sc = sa->AddComponent<ScenesComponent>();
	sc->Load("sushi_bar/scene.gltf");
	sc->FieldExpand();
	//sc->animationInstances[0].weight = 1.f;
	//sc->animationInstances[0].time = (sin(window.time) * 0.5 + 0.5) * 2;
	//sc->animationInstances[0].time = 1.f;
	ResetWorldMatrix(sa);
	for (int i = 0; i < sc->nodes.size(); i++)
	{
		MeshComponent* mc = sc->nodes[i]->GetComponent<MeshComponent>();
		if (!mc)
		{
			ScenesComponent* dbg = sc->nodes[i]->AddComponent<ScenesComponent>();
			dbg->Load("blender/SmallCube.gltf");
			dbg->FieldExpand();
		}
		
	}
	

	//Actor* rja = (Actor*)FindNodeByName("mixamorig:LeftHandIndex1", sa);

	

	while (window.IsOpen())
	{
		window.Tick();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		GLClear();
		RenderActor(sa, c);

		if (window.keys[GLFW_KEY_ESCAPE].pressDown)
		{
			window.Close();
		}
	}
	delete sa;
	delete ca;
	return 0;
}