#ifndef RENDERER_H
#define RENDERER_H

#include "Core.h"
#include "Scenes.h"
#include "Camera.h"


class Renderer
{
public:
    vector<Actor*> actors;
    Material m;
    Camera c;

    void SetupActors(Actor* parent, Actor* current)
    {
        if (parent)
        {
            current->modelMatrix = current->localTransform.ToMatrix() * parent->modelMatrix;
        }
        else
        {
            current->modelMatrix = current->localTransform.ToMatrix();
        }
        for (list<unique_ptr<Actor>>::iterator it = current->childrenList.begin(); it != current->childrenList.end(); it++)
        {
            SetupActors(current, (*it).get());
        }
    }
    void SetupMaterial()
    {
        m.UseDefaultShader();
        m.shader->Use();
        
    }
    void Render(Actor* current)
    {
        MeshComponent* mc = current->GetComponent<MeshComponent>();
        if (mc)
        {
            mat4 M = current->modelMatrix;
            mat4 V = translate(mat4(1.0f), vec3(0,0,-10.f));
            mat4 P = c.perspective.GetPerspectiveMatrix();
            
            for (int i = 0; i < mc->mesh->primitives.size(); i++)
            {

                mc->mesh->primitives[i].Draw();
            }
        }
        for (list<unique_ptr<Actor>>::iterator it = current->childrenList.begin(); it != current->childrenList.end(); it++)
        {
            Render((*it).get());
        }
    }
};

#endif