#ifndef RENDER_H
#define RENDER_H

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"
#include "Gameplay/Component/CameraComponent.h"
#include "Gameplay/Component/SkinComponent.h"
#include "OS/Window/Window.h"

#include "Resource/Texture/TextureCube.h"

class RenderUniform
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

    RenderUniform* uniform = nullptr;

    
	
    void Draw()
    {
        shared_ptr<Shader> s = material->shader;
    	s->Use();
    	s->SetMat4("M", *worldMatrix);
    	s->SetMat4("V", uniform->V);
    	s->SetMat4("P", uniform->P);

        s->SetVec3("viewPos", uniform->viewPos);
        s->SetVec3("lightDir", uniform->lightDir);
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

void RenderQuery(Actor* root, map<MaterialAlphaMode, vector<RenderPrimitive>>& renderPrimitiveMap, RenderUniform& uniform)
{
    uniform.cameraComponent = nullptr;
    renderPrimitiveMap.clear();
    root->RForEachNode<Actor>([&renderPrimitiveMap, &uniform](Actor* actor){
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
                rp.uniform = &uniform;
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
        if (!uniform.cameraComponent)
        {
            CameraComponent* cameraComponent = actor->GetComponent<CameraComponent>();
            if (cameraComponent)
            {
                uniform.cameraComponent = cameraComponent;
            }
        }
    });
}
shared_ptr<GLTexture2D> MakeTexture(GLsizei width, GLsizei height, GLint internalformat = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, const void *pixels = NULL, GLint border = 0)
{
    shared_ptr<GLTexture2D> t = make_shared<GLTexture2D>();
    t->TexImage2D(t->GetTarget(), 0, internalformat, width, height, 0, format, type, pixels);
    t->Filters(GL_NEAREST, GL_NEAREST);
    t->WrapST(GL_REPEAT, GL_REPEAT);
    return t;
}
class Render
{
public:
    Window* window = GetCurrentWindowContext();
    map<MaterialAlphaMode, vector<RenderPrimitive>> renderPrimitiveMap;
    RenderUniform uniform;

    shared_ptr<GLTexture2D> gColor;
    shared_ptr<GLTexture2D> gPosition;
    shared_ptr<GLTexture2D> gNormal;
    GLRenderBuffer gDepth;
    GLFrameBuffer gBuffer;

    // SSAO
    shared_ptr<GLTexture2D> tNoise;
    shared_ptr<GLTexture2D> tSSAO;
    GLFrameBuffer ssaoBuffer;
    shared_ptr<Material> mSSAO;

    vector<vec3> randomSphere;
    shared_ptr<MeshPrimitive> quad = MakeQuadMeshPrimitive(MakeMaterial("screen"));

    Render()
    {
        ivec2 s = window->GetFrameBufferSize();
        gPosition = MakeTexture(s.x, s.y, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        gNormal = MakeTexture(s.x, s.y, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        gColor = MakeTexture(s.x, s.y, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        gBuffer.DrawColorBuffers(3);
        gDepth.Storage(GL_DEPTH_COMPONENT, s.x, s.y);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT0, gPosition->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT1, gNormal->id);
        gBuffer.Texture2D(GL_COLOR_ATTACHMENT2, gColor->id);
        gBuffer.Renderbuffer(GL_DEPTH_ATTACHMENT, gDepth.id);
        
        vector<vec3> ssaoNoise = RandomKernel2D(16);
        tNoise = MakeTexture(4, 4, GL_RGBA16F, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        tSSAO = MakeTexture(s.x, s.y, GL_RED, GL_RED, GL_FLOAT);
        ssaoBuffer.Texture2D(GL_COLOR_ATTACHMENT0, tSSAO->id);
        mSSAO = MakeMaterial("ssao");
        randomSphere = RandomSphere(64);
    }
    void Load(Actor* root)
    {
        RenderQuery(root, renderPrimitiveMap, uniform);
    }
    void Draw()
    {
        uniform.Reset();
        
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
        //ssaoBuffer.Clear(GL_COLOR_BUFFER_BIT);
    
        mSSAO->textureMap["tB"] = gColor.get();
        mSSAO->textureMap["tP"] = gPosition.get();
        mSSAO->textureMap["tN"] = gNormal.get();
        mSSAO->shader->SetMat4("V", uniform.V);
        mSSAO->shader->SetMat4("P", uniform.P);
        mSSAO->shader->SetVec3("viewPos", uniform.viewPos);
        for (size_t i = 0; i < randomSphere.size(); i++)
        {
            mSSAO->shader->SetVec3("rands["+to_string(i)+"]", randomSphere[i]);
        }
        mSSAO->SetUniform();
        quad->Draw();
        
        // glContext.ClearColor(0.f, 0.f, 0.f, 0.f);
        // glContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // quad->material->textureMap["tB"] = gColor.get();
        // quad->material->textureMap["tP"] = gPosition.get();
        // quad->material->textureMap["tN"] = gNormal.get();
        // quad->material->shader->Use();
        // quad->material->SetUniform();
        // quad->Draw();
    }
    
};

#endif