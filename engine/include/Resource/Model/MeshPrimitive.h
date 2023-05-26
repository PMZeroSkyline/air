#ifndef MESH_PRIMITIVE_H
#define MESH_PRIMITIVE_H

#include "SDK/OpenGL/CppOpenGL.h"
#include "MeshAttribute.h"
#include "SDK/OpenGL/MeshCommit.h"
#include "Resource/Material/Material.h"
#include "Physic/Shape/BoundingBox.h"

class MeshPrimitive
{
public:
    GLPrimitive glPrimitive;
    MeshAttribute attribute;
    vector<unsigned int> indices;
    shared_ptr<Material> material;
    BoundingBox boundingBox;

    void SetupGLPrimitive()
    {
        LOG("SetupGLPrimitive")
        glPrimitive.Bind();
        GLVaoData(attribute.POSITION, attribute.NORMAL, attribute.TANGENT, attribute.TEXCOORD_0, attribute.TEXCOORD_1, attribute.TEXCOORD_2, attribute.TEXCOORD_3, attribute.JOINTS_0, attribute.WEIGHTS_0);
        GLEboData(indices);
    }
    void Draw()
    {
        material->Bind();
        glPrimitive.Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    
};
shared_ptr<MeshPrimitive> MakeQuadMeshPrimitive()
{
    shared_ptr<MeshPrimitive> primitive = make_shared<MeshPrimitive>();
    primitive->material = make_shared<Material>();
    primitive->material->UseDefaultShader();
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
    primitive->SetupGLPrimitive();
    return primitive;
}
shared_ptr<MeshPrimitive> MakeCubeMeshPrimitive()
{
    shared_ptr<MeshPrimitive> primitive = make_shared<MeshPrimitive>();
    primitive->material = make_shared<Material>();
    primitive->material->UseDefaultShader();
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
    primitive->SetupGLPrimitive();
    return primitive;
}
#endif