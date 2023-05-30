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
    float alphaCutoff = 0.5f;
    MaterialAlphaMode alphaMode = MaterialAlphaMode::OPAQUE;
    bool doubleSided = true;
    GLFaceMode faceMode = GLFaceMode::FILL;
    bool depthTest = true;

    map<string, GLTexture2D*> textureMap;
    map<string, mat4> matrixMap;
    map<string, vector<mat4>> matricesMap;

    
    void Use()
    {
        // Setup Render Context
        bool isCullFace = !doubleSided;
        if (renderContext.isCullFace != isCullFace)
        {
            GLSetCullFace(isCullFace);
            renderContext.isCullFace = isCullFace;
        }
        bool isBlend = alphaMode == MaterialAlphaMode::BLEND;
        if (renderContext.isBlend != isBlend)
        {
            GLSetBlend(isBlend);
            renderContext.isBlend = isBlend;
        }
        if (renderContext.frontAndBackFaceMode != faceMode)
        {
            GLSetFaceMode(faceMode);
            renderContext.frontAndBackFaceMode = faceMode;
        }
        if (renderContext.depthTest != depthTest)
        {
            GLSetDepthTest(depthTest);
            renderContext.depthTest = depthTest;
        }
        

        shader->Use();

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

#endif