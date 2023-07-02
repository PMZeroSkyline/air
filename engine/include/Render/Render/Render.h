#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/SkinComponent.h"
#include "OS/Window/Window.h"

#include "Resource/Texture/TextureCube.h"

class RenderInput
{
public:
    CameraComponent* cameraComponent = nullptr;
    mat4 V;
    mat4 P;
    vec3 viewPos;
    vec3 lightDir;
    void Reset()
    {
        Actor* aCamera = ((Actor*)cameraComponent->owner);
        V = RightHandZUpToYUpProjection() * aCamera->worldMatrix.inverse();
        P = cameraComponent->camera->GetProjectioMatrix();
        viewPos = ToVec3(aCamera->worldMatrix.column(3));
        lightDir = vec3(1.f).normalize();
    }
};
class RenderPrimitive
{
public:
    mat4* parentWorldMatrix = nullptr;
    mat4* worldMatrix = nullptr;
    MeshPrimitive* meshPrimitive = nullptr;
    SkinInstance* skinInstance = nullptr;
    Material* material = nullptr;

    RenderInput* input = nullptr;

    
	
    void Draw()
    {
        shared_ptr<Shader> s = material->shader;
    	s->Use();
    	s->SetMat4("M", *worldMatrix);
    	s->SetMat4("V", input->V);
    	s->SetMat4("P", input->P);

        s->SetVec3("viewPos", input->viewPos);
        s->SetVec3("lightDir", input->lightDir);
    	s->SetBool("isSkin", false);

    	if (skinInstance)
    	{
    		Skin* skin = skinInstance->skin;
    		vector<Actor*> bones = skinInstance->joints;
    		mat4 IW = parentWorldMatrix ? parentWorldMatrix->inverse() : mat4();
    		vector<mat4> jms;
    		for (int i = 0; i < bones.size(); i++)
    		{
    			Actor* bone = bones[i];
    			mat4 B = IW * bone->worldMatrix * skin->inverseBindMatrices[i];
    			s->SetMat4("J[" + to_string(i) + "]", B);
    		}
    	}
        material->SetUniform();
    	material->ResetRenderContext();
    	meshPrimitive->Draw();
    }
};

void RenderQuery(Actor* root, map<MaterialAlphaMode, vector<RenderPrimitive>>& renderPrimitiveMap, RenderInput& input)
{
    input.cameraComponent = nullptr;
    renderPrimitiveMap.clear();
    root->RForEachNode<Actor>([&renderPrimitiveMap, &input](Actor* actor){
        MeshComponent* mc = actor->GetComponent<MeshComponent>();
        if (mc)
        {
            SkinComponent* sc = actor->GetComponent<SkinComponent>();
            Actor* owner = (Actor*)mc->owner;
            Actor* parent = owner ? ((Actor*)owner->parent) : nullptr;
            for (shared_ptr<MeshPrimitive> meshPrimitive : mc->mesh->primitives)
            {
                RenderPrimitive rp;
                rp.meshPrimitive = meshPrimitive.get();
                rp.material = meshPrimitive->material.get();
                rp.skinInstance = sc ? sc->skinInstance : nullptr;
                rp.input = &input;
                if (owner)
                {
                    rp.worldMatrix = &owner->worldMatrix;
                }
                if (mc->owner->parent)
                {
                    rp.parentWorldMatrix = &parent->worldMatrix;
                }
                renderPrimitiveMap[rp.material->alphaMode].push_back(rp);
            }
        }
        if (!input.cameraComponent)
        {
            CameraComponent* cameraComponent = actor->GetComponent<CameraComponent>();
            if (cameraComponent)
            {
                input.cameraComponent = cameraComponent;
            }
        }
    });
}

class Render
{
public:
    Window* window = GetCurrentWindowContext();
    map<MaterialAlphaMode, vector<RenderPrimitive>> renderPrimitiveMap;
    RenderInput input;

    Render()
    {
        // glDisable(GL_BLEND);
        // glEnable(GL_DEPTH_TEST);
        // glDepthMask(GL_TRUE);
        // glEnable(GL_CULL_FACE);
    }
    void Load(Actor* root)
    {
        RenderQuery(root, renderPrimitiveMap, input);
        
    }
    void Draw()
    {
        input.Reset();
        
        glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	for (int i = 0; i != renderPrimitiveMap[MaterialAlphaMode::OPAQUE].size(); i++)
        {
            renderPrimitiveMap[MaterialAlphaMode::OPAQUE][i].Draw();
        }
        for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::MASK])
        {
            rp.Draw();
        }
        for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::BLEND])
        {
            rp.Draw();
        }
    }
    
};

#endif