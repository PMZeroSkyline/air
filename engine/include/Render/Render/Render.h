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
    	s->SetBool("bSkin", skinInstance ? true : false);
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
        s->SetInt("bMask", material->alphaMode == MaterialAlphaMode::MASK);
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
shared_ptr<GLTexture2D> MakeUByteNearestTex(ivec2 size)
{
    shared_ptr<GLTexture2D> t = make_shared<GLTexture2D>();
    t->TexImage2D(t->GetTarget(), 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    t->Filters(GL_NEAREST, GL_NEAREST);
    return t;
}
shared_ptr<GLTexture2D> MakeFloatNearestTex(ivec2 size)
{
    shared_ptr<GLTexture2D> t = make_shared<GLTexture2D>();
    t->TexImage2D(t->GetTarget(), 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
    t->Filters(GL_NEAREST, GL_NEAREST);
    return t;
}
class Render
{
public:
    Window* window = GetCurrentWindowContext();
    map<MaterialAlphaMode, vector<RenderPrimitive>> renderPrimitiveMap;
    RenderInput input;

    shared_ptr<GLTexture2D> gColor;
    shared_ptr<GLTexture2D> gPosition;
    shared_ptr<GLTexture2D> gNormal;
    GLRenderBuffer gDepth;
    GLFrameBuffer gBuffer;

    shared_ptr<MeshPrimitive> quad = MakeQuadMeshPrimitive(MakeMaterial("screen"));

    Render()
    {
        ivec2 size = window->GetFrameBufferSize();
        gPosition = MakeFloatNearestTex(size);
        gNormal = MakeFloatNearestTex(size);
        gColor = MakeUByteNearestTex(size);
        gBuffer.DrawColorBuffers(3);
        gDepth.Storage(GL_DEPTH_COMPONENT, size.x, size.y);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT0, gPosition->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT1, gNormal->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT2, gColor->id);
        gBuffer.Renderbuffer(GL_DEPTH_ATTACHMENT, gDepth.id);
        LOG(gBuffer.IsComplete())
    }
    void Load(Actor* root)
    {
        RenderQuery(root, renderPrimitiveMap, input);
    }
    void Draw()
    {
        input.Reset();
        
        gBuffer.ClearColor(0.f, 0.f, 0.f, 0.f);
        gBuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::OPAQUE])
        {
            rp.Draw();
        }
        for (auto& rp : renderPrimitiveMap[MaterialAlphaMode::MASK])
        {
            rp.Draw();
        }
        
        glContext.ClearColor(0.f, 0.f, 0.f, 0.f);
        glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad->material->textureMap["tB"] = gColor.get();
        quad->material->textureMap["tP"] = gPosition.get();
        quad->material->textureMap["tN"] = gNormal.get();
        quad->material->shader->Use();
        quad->material->SetUniform();
        quad->Draw();
    }
    
};

#endif