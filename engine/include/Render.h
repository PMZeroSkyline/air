#ifndef RENDER_H
#define RENDER_H

#include "Core.h"
#include "Scenes.h"

class RenderObject
{
public:
    MeshPrimitive* meshPrimitive;
    mat4* modelMatrix;
};
class RenderContext
{
public:
    vector<RenderObject*> renderObjects;
};
class Renderer
{
public:
    RenderContext* context;
    void Render()
    {

    }
};

#endif