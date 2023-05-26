#ifndef MESH_PRIMITIVE_H
#define MESH_PRIMITIVE_H

#include "SDK/OpenGL/CppOpenGL.h"
#include "MeshAttribute.h"
#include "SDK/OpenGL/MeshCommit.h"
#include "Resource/Material/Material.h"
#include "Physic/Shape/AABB.h"

class MeshPrimitive
{
public:
    GLPrimitive glPrimitive;
    MeshAttribute attribute;
    vector<unsigned int> indices;
    shared_ptr<Material> material;
    AABB aabb;

    void SetupGLPrimitive()
    {
        glPrimitive.Bind();
        GLVaoData(attribute.POSITION, attribute.NORMAL, attribute.TANGENT, attribute.TEXCOORD_0, attribute.TEXCOORD_1, attribute.TEXCOORD_2, attribute.TEXCOORD_3, attribute.JOINTS_0, attribute.WEIGHTS_0);
        GLEboData(indices);
    }
    void Draw()
    {
        material->Use();
        glPrimitive.Bind();
        GLDrawElements(indices.size());
    }
    
};
shared_ptr<MeshPrimitive> MakeQuadMeshPrimitive(shared_ptr<Material> material = shared_ptr<Material>())
{
    shared_ptr<MeshPrimitive> primitive = make_shared<MeshPrimitive>();
    if (material)
    {
        primitive->material = material;
    }
    else
    {
        primitive->material = make_shared<Material>();
        primitive->material->shader = GetPresetShader("default");
    }
    primitive->attribute.POSITION = {
        {-1.0f, -1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},
        {-1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f}
    };
    primitive->attribute.TEXCOORD_0 = {
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0, 1.0f},
        {1.0f, 0.0f},
    };
    primitive->attribute.NORMAL = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };
    primitive->attribute.TANGENT = {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f}
    };
    primitive->indices = {0, 1, 3, 0, 3, 2};
    primitive->aabb = AABB(vec3(-1.f, -1.f, 0.f), vec3(1.f, 1.f, 0.f));
    primitive->SetupGLPrimitive();
    return primitive;
}
shared_ptr<MeshPrimitive> MakeCubeMeshPrimitive(shared_ptr<Material> material = shared_ptr<Material>())
{
    shared_ptr<MeshPrimitive> primitive = make_shared<MeshPrimitive>();
    if (material)
    {
        primitive->material = material;
    }
    else
    {
        primitive->material = make_shared<Material>();
        primitive->material->shader = GetPresetShader("default");
    }
    primitive->indices = {
        2, 4, 9,
        2, 9, 7,
        8, 10, 22,
        8, 22, 19,
        20, 23, 17,
        20, 17, 14,
        12, 15, 3,
        12, 3, 1,
        6, 18, 13,
        6, 13, 0,
        21, 11, 5,
        21, 5, 16
    };
    primitive->attribute.POSITION = {
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f},
        {-1.0f, -1.0f, 1.0f},
        {-1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    };
    primitive->attribute.TEXCOORD_0 = {
        {0.125f, 0.25f},
        {0.375f, 0.00f},
        {0.375f, 1.00f},
        {0.625f, 0.00f},
        {0.625f, 1.00f},
        {0.875f, 0.25f},
        {0.125f, 0.50f},
        {0.375f, 0.75f},
        {0.375f, 0.75f},
        {0.625f, 0.75f},
        {0.625f, 0.75f},
        {0.875f, 0.50f},
        {0.375f, 0.25f},
        {0.375f, 0.25f},
        {0.375f, 0.25f},
        {0.625f, 0.25f},
        {0.625f, 0.25f},
        {0.625f, 0.25f},
        {0.375f, 0.50f},
        {0.375f, 0.50f},
        {0.375f, 0.50f},
        {0.625f, 0.50f},
        {0.625f, 0.50f},
        {0.625f, 0.50f}
    };
    primitive->attribute.NORMAL = {
        {0.0f, 0.0f, -1.0f},
        {0.0f, -1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, -1.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
    };
    primitive->attribute.TANGENT = {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {-1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {-1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {-1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {-1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    };
    primitive->aabb = AABB(vec3(-1.f), vec3(1.f));
    primitive->SetupGLPrimitive();
    return primitive;
}
#endif