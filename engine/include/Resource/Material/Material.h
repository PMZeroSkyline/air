#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Resource/Texture/Texture2D.h"

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
    int alphaMode = MaterialAlphaMode::OPAQUE;
    bool doubleSided = true;

    vector<pair<string, shared_ptr<Texture2D>>> texturePairs;
    map<string, mat4> mat4Map;
    vector<mat4> jointMatrix;

    void UseDefaultShader()
    {
        shader = shaderBlob.Get("default");
        if (!shader)
        {
            shader = make_shared<Shader>();
            shader->Load("shader/default_vs.glsl", "shader/default_fs.glsl");
            shaderBlob.Set("default", shader);
        }
    }
    void Bind()
    {
        shader->Use();
        //for (auto it = mat4PtrMap.begin(); it != mat4PtrMap.end(); it++)
        //{
        //    shader->SetMat4(it->first, *it->second);
        //}
        for (auto it = mat4Map.begin(); it != mat4Map.end(); it++)
        {
            shader->SetMat4(it->first, it->second);
        }
        shader->SetBool("isSkin", jointMatrix.size() != 0);
        for (int i = 0; i < jointMatrix.size(); i++)
        {
            shader->SetMat4("J[" + to_string(i) + "]", jointMatrix[i]);
        }
        
        
        for (int i = 0; i < texturePairs.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texturePairs[i].second->glTexture2D.id);
        }
        for (int i = 0; i < texturePairs.size(); i++)
        {
            shader->SetInt(texturePairs[i].first, i);
        }
    }
};

#endif