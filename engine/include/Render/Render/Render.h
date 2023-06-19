#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"
#include "Gameplay/Component/CameraComponent.h"
#include "OS/Window/Window.h"

class RenderPrimitive
{
public:
    mat4* parentWorldMatrix = nullptr;
    mat4* worldMatrix = nullptr;
    MeshPrimitive* meshPrimitive = nullptr;
    SkinInstance* skinInstance = nullptr;
    Material* material = nullptr;
    void DrawMVP(const mat4& V, const mat4& P, const vec3& viewPos, const vec3& lightDir, shared_ptr<Shader> shader = shared_ptr<Shader>())
    {
        shared_ptr<Shader> s = shader ? shader : material->shader;
    	s->Use();
    	s->SetMat4("M", *worldMatrix);
    	s->SetMat4("V", V);
    	s->SetMat4("P", P);
        s->SetVec3("viewPos", viewPos);
        s->SetVec3("lightDir", lightDir);
    	s->SetBool("isSkin", skinInstance);
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

CameraComponent* RenderQuery(Actor* root, vector<RenderPrimitive>& masks, vector<RenderPrimitive>& blends)
{
    CameraComponent* cCamera = nullptr;
    root->RForEachNode<Actor>([&masks, &blends, &cCamera](Actor* actor){
        vector<MeshComponent*> mcs;
        actor->GetComponents<MeshComponent>(mcs);
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
                if (owner)
                {
                    rp.worldMatrix = &owner->worldMatrix;
                }
                if (mc->owner->parent)
                {
                    rp.parentWorldMatrix = &parent->worldMatrix;
                }
                if (rp.material->alphaMode == MaterialAlphaMode::BLEND)
                {
                    blends.push_back(rp);
                }
                else
                {
                    masks.push_back(rp);
                }
            }
        }
        if (!cCamera)
        {
            CameraComponent* cam = actor->GetComponent<CameraComponent>();
            if (cam)
            {
                cCamera = cam;
            }
        }
    });
    return cCamera;
}

class Render
{
public:
    Window* window = GetCurrentWindowContext();
    vector<RenderPrimitive> masks, blends;
    CameraComponent* cCamera = nullptr;
    mat4 V;
    mat4 P;
    vec3 viewPos;
    vec3 lightDir = vec3(1.f).normalize();
    Render()
    {
    }
    void Load(Actor* world)
    {
        masks.clear();
        blends.clear();
        cCamera = RenderQuery(world, masks, blends);
        Actor* aCamera = (Actor*)cCamera->owner;
        V = RightHandZUpToYUpProjection() * aCamera->worldMatrix.inverse();
        P = cCamera->camera->GetProjectioMatrix();
        viewPos = ToVec3(aCamera->worldMatrix.column(3));
    }
    void DrawForward()
    {
        glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	for (RenderPrimitive& rp : masks)
    	{
    		rp.DrawMVP(V, P, viewPos, lightDir);
    	}
        for (RenderPrimitive& rp : blends)
    	{
    		rp.DrawMVP(V, P, viewPos, lightDir);
    	}
    }
    void Draw()
    {
        DrawForward();
    }
};

#endif