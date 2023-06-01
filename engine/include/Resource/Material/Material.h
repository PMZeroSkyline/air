#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Resource/Texture/Texture2D.h"
#include "Render/Render/RenderContext.h"

enum MaterialAlphaMode
{
    OPAQUE,
    MASK,
    BLEND
};

class Material
{
public:
    string name;
    shared_ptr<Shader> shader;

    // Params
    float alphaCutoff = 0.5f;
    map<string, GLTexture2D*> textureMap;
    map<string, mat4> matrixMap;
    map<string, vector<mat4>> matricesMap;

    // Render Context
    MaterialAlphaMode alphaMode = MaterialAlphaMode::OPAQUE;
    bool depthTest = true;
    bool depthMask = true;
    bool doubleSided = false;
    GLPolygonMode faceMode = GLPolygonMode::FILL;


    void ResetRenderContext()
    {
        bool isCullFace = !doubleSided;
        if (renderContext.cullFace != isCullFace)
        {
            GLCullFace(isCullFace);
            renderContext.cullFace = isCullFace;
        }
        bool isBlend = alphaMode == MaterialAlphaMode::BLEND;
        if (renderContext.blend != isBlend)
        {
            GLBlend(isBlend);
            renderContext.blend = isBlend;
            
        }
        if (renderContext.frontAndBackFaceMode != faceMode)
        {
            GLPolygon(faceMode);
            renderContext.frontAndBackFaceMode = faceMode;
        }
        if (renderContext.depthTest != depthTest)
        {
            GLDepthTest(depthTest);
            renderContext.depthTest = depthTest;
        }
        if (renderContext.depthMask != depthMask)
        {
            GLDepthMask(depthMask);
            renderContext.depthMask = depthMask;
        }
        
        
    }
    void Setup()
    {        
        for (auto const& pair : matricesMap)
        {
            for (int i = 0; i < pair.second.size(); i++)
            {
                shader->SetMat4(pair.first + "[" + to_string(i) + "]" , pair.second[i]);
            }            
        }
        for (auto const& pair : matrixMap)
        {
            shader->SetMat4(pair.first, pair.second);
        }

        int textureIndex = 0;
        for (auto const& pair : textureMap)
        {
            GLActiveTexture(textureIndex);
            pair.second->Bind();
            shader->SetInt(pair.first, textureIndex);
            textureIndex++;
        }
    }
};

shared_ptr<Material> MakeMaterialFromShaderRes(const string& name)
{
    shared_ptr<Material> material = make_shared<Material>();
    material->shader = MakeShaderFromRes(name);
    return material;
}
#endif