#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Resource/Texture/Texture2D.h"

enum MaterialSort
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
    MaterialSort sortMode = MaterialSort::OPAQUE;
    bool doubleSided = true;

    vector<pair<string, shared_ptr<Texture2D>>> texturePairs;
    map<string, mat4> mat4Map;
    map<string, mat4*> mat4PtrMap;
    vector<mat4> jointMatrix;

    bool operator<(const Material& rhs) const
    {
        return sortMode < rhs.sortMode;
    }

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
        for (auto it = mat4PtrMap.begin(); it != mat4PtrMap.end(); it++)
        {
           shader->SetMat4(it->first, *it->second);
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
            shader->SetInt(texturePairs[i].first, i);
        }
        shader->SetBool("isUseTexture", texturePairs.size() != 0);
    }
};

#endif