#ifndef MESH_PRIMITIVE_H
#define MESH_PRIMITIVE_H

#include "SDK/OpenGL/CppOpenGL.h"
#include "MeshAttribute.h"
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
        glPrimitive.VertexAttributeData(attribute.POSITION, attribute.NORMAL, attribute.TANGENT, attribute.TEXCOORD_0, attribute.TEXCOORD_1, attribute.TEXCOORD_2, attribute.TEXCOORD_3, attribute.JOINTS_0, attribute.WEIGHTS_0);
        glPrimitive.ebo->Data(sizeof(unsigned int) * indices.size(), &indices[0]);
    }
    void Draw()
    {
        glPrimitive.DrawElements(indices.size());
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
        primitive->material->shader = MakeShaderFromPresetDir("default");
    }
    primitive->attribute.POSITION = {
        {-1.00000, -1.00000, 0.00000},
        {1.00000, -1.00000, 0.00000},
        {-1.00000, 1.00000, 0.00000},
        {1.00000, 1.00000, 0.00000}
    };
    primitive->attribute.TEXCOORD_0 = {
        {0.00000, 0.00000},
        {1.00000, 0.00000},
        {0.00000, 1.00000},
        {1.00000, 1.00000}
    };
    primitive->attribute.NORMAL = {
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000}
    };
    primitive->attribute.TANGENT = {
        {1.00000, 0.00000, 0.00000, 1.00000},
        {1.00000, 0.00000, 0.00000, 1.00000},
        {1.00000, 0.00000, 0.00000, 1.00000},
        {1.00000, 0.00000, 0.00000, 1.00000}
    };
    primitive->indices = {
        0, 1, 3,
        0, 3, 2
    };
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
        primitive->material->shader = MakeShaderFromPresetDir("default");
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
shared_ptr<MeshPrimitive> MakeCapsulePrimitive(shared_ptr<Material> material = shared_ptr<Material>())
{
    shared_ptr<MeshPrimitive> primitive = make_shared<MeshPrimitive>();
    if (material)
    {
        primitive->material = material;
    }
    else
    {
        primitive->material = make_shared<Material>();
        primitive->material->shader = MakeShaderFromPresetDir("default");
    }
    primitive->indices = {
        78, 77, 5,
        78, 5, 6,
        76, 75, 3,
        76, 3, 4,
        74, 73, 1,
        74, 1, 2,
        72, 69, 0,
        61, 78, 6,
        77, 76, 4,
        77, 4, 5,
        87, 74, 2,
        87, 2, 80,
        73, 72, 0,
        73, 0, 1,
        60, 6, 13,
        5, 4, 11,
        5, 11, 12,
        80, 2, 9,
        80, 9, 79,
        1, 0, 7,
        1, 7, 8,
        6, 5, 12,
        6, 12, 13,
        4, 3, 10,
        4, 10, 11,
        2, 1, 8,
        2, 8, 9,
        0, 68, 7,
        79, 9, 16,
        79, 16, 81,
        8, 7, 14,
        8, 14, 15,
        13, 12, 19,
        13, 19, 20,
        11, 10, 17,
        11, 17, 18,
        9, 8, 15,
        9, 15, 16,
        7, 67, 14,
        59, 13, 20,
        12, 11, 18,
        12, 18, 19,
        20, 19, 26,
        20, 26, 27,
        18, 17, 24,
        18, 24, 25,
        16, 15, 22,
        16, 22, 23,
        14, 66, 21,
        58, 20, 27,
        19, 18, 25,
        19, 25, 26,
        81, 16, 23,
        81, 23, 82,
        15, 14, 21,
        15, 21, 22,
        23, 22, 29,
        23, 29, 30,
        21, 65, 28,
        57, 27, 34,
        26, 25, 32,
        26, 32, 33,
        82, 23, 30,
        82, 30, 83,
        22, 21, 28,
        22, 28, 29,
        27, 26, 33,
        27, 33, 34,
        25, 24, 31,
        25, 31, 32,
        56, 34, 47,
        33, 32, 43,
        33, 43, 45,
        83, 30, 39,
        83, 39, 84,
        29, 28, 35,
        29, 35, 37,
        34, 33, 45,
        34, 45, 47,
        32, 31, 41,
        32, 41, 43,
        30, 29, 37,
        30, 37, 39,
        28, 64, 35,
        38, 36, 49,
        38, 49, 50,
        48, 46, 54,
        48, 54, 55,
        44, 42, 52,
        44, 52, 53,
        40, 38, 50,
        40, 50, 51,
        36, 71, 49,
        63, 48, 55,
        46, 44, 53,
        46, 53, 54,
        85, 40, 51,
        85, 51, 86,
        55, 54, 77,
        55, 77, 78,
        53, 52, 75,
        53, 75, 76,
        51, 50, 73,
        51, 73, 74,
        49, 70, 72,
        62, 55, 78,
        54, 53, 76,
        54, 76, 77,
        86, 51, 74,
        86, 74, 87,
        50, 49, 72,
        50, 72, 73,
        3, 75, 87,
        3, 87, 80,
        75, 52, 86,
        75, 86, 87,
        52, 42, 85,
        52, 85, 86,
        41, 31, 83,
        41, 83, 84,
        31, 24, 82,
        31, 82, 83,
        24, 17, 81,
        24, 81, 82,
        17, 10, 79,
        17, 79, 81,
        10, 3, 80,
        10, 80, 79,
    };
    primitive->attribute.POSITION = {
        {0.27060, 0.27060, 1.92388},
        {0.50000, 0.50000, 1.70711},
        {0.65328, 0.65328, 1.38268},
        {0.70711, 0.70711, -1.00000},
        {0.65328, 0.65328, -1.38268},
        {0.50000, 0.50000, -1.70711},
        {0.27060, 0.27060, -1.92388},
        {0.38268, 0.00000, 1.92388},
        {0.70711, 0.00000, 1.70711},
        {0.92388, 0.00000, 1.38268},
        {1.00000, 0.00000, -1.00000},
        {0.92388, 0.00000, -1.38268},
        {0.70711, 0.00000, -1.70711},
        {0.38268, 0.00000, -1.92388},
        {0.27060, -0.27060, 1.92388},
        {0.50000, -0.50000, 1.70711},
        {0.65328, -0.65328, 1.38268},
        {0.70711, -0.70711, -1.00000},
        {0.65328, -0.65328, -1.38268},
        {0.50000, -0.50000, -1.70711},
        {0.27060, -0.27060, -1.92388},
        {0.00000, -0.38268, 1.92388},
        {0.00000, -0.70711, 1.70711},
        {0.00000, -0.92388, 1.38268},
        {0.00000, -1.00000, -1.00000},
        {0.00000, -0.92388, -1.38268},
        {0.00000, -0.70711, -1.70711},
        {0.00000, -0.38268, -1.92388},
        {-0.27060, -0.27060, 1.92388},
        {-0.50000, -0.50000, 1.70711},
        {-0.65328, -0.65328, 1.38268},
        {-0.70711, -0.70711, -1.00000},
        {-0.65328, -0.65328, -1.38268},
        {-0.50000, -0.50000, -1.70711},
        {-0.27060, -0.27060, -1.92388},
        {-0.38268, 0.00000, 1.92388},
        {-0.38268, 0.00000, 1.92388},
        {-0.70711, 0.00000, 1.70711},
        {-0.70711, 0.00000, 1.70711},
        {-0.92388, 0.00000, 1.38268},
        {-0.92388, 0.00000, 1.38268},
        {-1.00000, 0.00000, -1.00000},
        {-1.00000, 0.00000, -1.00000},
        {-0.92388, 0.00000, -1.38268},
        {-0.92388, 0.00000, -1.38268},
        {-0.70711, 0.00000, -1.70711},
        {-0.70711, 0.00000, -1.70711},
        {-0.38268, 0.00000, -1.92388},
        {-0.38268, 0.00000, -1.92388},
        {-0.27060, 0.27060, 1.92388},
        {-0.50000, 0.50000, 1.70711},
        {-0.65328, 0.65328, 1.38268},
        {-0.70711, 0.70711, -1.00000},
        {-0.65328, 0.65328, -1.38268},
        {-0.50000, 0.50000, -1.70711},
        {-0.27060, 0.27060, -1.92388},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, -2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.00000, 2.00000},
        {0.00000, 0.38268, 1.92388},
        {0.00000, 0.70711, 1.70711},
        {0.00000, 0.92388, 1.38268},
        {0.00000, 1.00000, -1.00000},
        {0.00000, 0.92388, -1.38268},
        {0.00000, 0.70711, -1.70711},
        {0.00000, 0.38268, -1.92388},
        {1.00000, 0.00000, 1.00000},
        {0.70711, 0.70711, 1.00000},
        {0.70711, -0.70711, 1.00000},
        {0.00000, -1.00000, 1.00000},
        {-0.70711, -0.70711, 1.00000},
        {-1.00000, 0.00000, 1.00000},
        {-1.00000, 0.00000, 1.00000},
        {-0.70711, 0.70711, 1.00000},
        {0.00000, 1.00000, 1.00000}
    };
    primitive->attribute.TEXCOORD_0 = {
        {0.62500, 0.06250},
        {0.62500, 0.12500},
        {0.62500, 0.18750},
        {0.62500, 0.75000},
        {0.62500, 0.81250},
        {0.62500, 0.87500},
        {0.62500, 0.93750},
        {0.50000, 0.06250},
        {0.50000, 0.12500},
        {0.50000, 0.18750},
        {0.50000, 0.75000},
        {0.50000, 0.81250},
        {0.50000, 0.87500},
        {0.50000, 0.93750},
        {0.37500, 0.06250},
        {0.37500, 0.12500},
        {0.37500, 0.18750},
        {0.37500, 0.75000},
        {0.37500, 0.81250},
        {0.37500, 0.87500},
        {0.37500, 0.93750},
        {0.25000, 0.06250},
        {0.25000, 0.12500},
        {0.25000, 0.18750},
        {0.25000, 0.75000},
        {0.25000, 0.81250},
        {0.25000, 0.87500},
        {0.25000, 0.93750},
        {0.12500, 0.06250},
        {0.12500, 0.12500},
        {0.12500, 0.18750},
        {0.12500, 0.75000},
        {0.12500, 0.81250},
        {0.12500, 0.87500},
        {0.12500, 0.93750},
        {0.00000, 0.06250},
        {1.00000, 0.06250},
        {0.00000, 0.12500},
        {1.00000, 0.12500},
        {0.00000, 0.18750},
        {1.00000, 0.18750},
        {0.00000, 0.75000},
        {1.00000, 0.75000},
        {0.00000, 0.81250},
        {1.00000, 0.81250},
        {0.00000, 0.87500},
        {1.00000, 0.87500},
        {0.00000, 0.93750},
        {1.00000, 0.93750},
        {0.87500, 0.06250},
        {0.87500, 0.12500},
        {0.87500, 0.18750},
        {0.87500, 0.75000},
        {0.87500, 0.81250},
        {0.87500, 0.87500},
        {0.87500, 0.93750},
        {0.06250, 1.00000},
        {0.18750, 1.00000},
        {0.31250, 1.00000},
        {0.43750, 1.00000},
        {0.56250, 1.00000},
        {0.68750, 1.00000},
        {0.81250, 1.00000},
        {0.93750, 1.00000},
        {0.06250, 0.00000},
        {0.18750, 0.00000},
        {0.31250, 0.00000},
        {0.43750, 0.00000},
        {0.56250, 0.00000},
        {0.68750, 0.00000},
        {0.81250, 0.00000},
        {0.93750, 0.00000},
        {0.75000, 0.06250},
        {0.75000, 0.12500},
        {0.75000, 0.18750},
        {0.75000, 0.75000},
        {0.75000, 0.81250},
        {0.75000, 0.87500},
        {0.75000, 0.93750},
        {0.50000, 0.25000},
        {0.62500, 0.25000},
        {0.37500, 0.25000},
        {0.25000, 0.25000},
        {0.12500, 0.25000},
        {0.00000, 0.25000},
        {1.00000, 0.25000},
        {0.87500, 0.25000},
        {0.75000, 0.25000}
    };
    primitive->attribute.NORMAL = {
        {0.29881, 0.29881, 0.90632},
        {0.51592, 0.51592, 0.68385},
        {0.65782, 0.65782, 0.36679},
        {0.70386, 0.70386, -0.09577},
        {0.65782, 0.65782, -0.36679},
        {0.51592, 0.51592, -0.68385},
        {0.29881, 0.29881, -0.90632},
        {0.42258, -0.00000, 0.90632},
        {0.72962, -0.00000, 0.68385},
        {0.93030, -0.00000, 0.36679},
        {0.99540, -0.00000, -0.09577},
        {0.93030, -0.00000, -0.36679},
        {0.72962, -0.00000, -0.68385},
        {0.42258, -0.00000, -0.90632},
        {0.29881, -0.29881, 0.90632},
        {0.51592, -0.51592, 0.68385},
        {0.65782, -0.65782, 0.36679},
        {0.70386, -0.70386, -0.09577},
        {0.65782, -0.65782, -0.36679},
        {0.51592, -0.51592, -0.68385},
        {0.29881, -0.29881, -0.90632},
        {-0.00000, -0.42258, 0.90632},
        {0.00000, -0.72962, 0.68385},
        {0.00000, -0.93030, 0.36679},
        {0.00000, -0.99540, -0.09577},
        {0.00000, -0.93030, -0.36679},
        {0.00000, -0.72962, -0.68385},
        {-0.00000, -0.42258, -0.90632},
        {-0.29881, -0.29881, 0.90632},
        {-0.51592, -0.51592, 0.68385},
        {-0.65782, -0.65782, 0.36679},
        {-0.70386, -0.70386, -0.09577},
        {-0.65782, -0.65782, -0.36679},
        {-0.51592, -0.51592, -0.68385},
        {-0.29881, -0.29881, -0.90632},
        {-0.42258, 0.00000, 0.90632},
        {-0.42258, 0.00000, 0.90632},
        {-0.72962, -0.00000, 0.68385},
        {-0.72962, -0.00000, 0.68385},
        {-0.93030, -0.00000, 0.36679},
        {-0.93030, -0.00000, 0.36679},
        {-0.99540, 0.00000, -0.09577},
        {-0.99540, 0.00000, -0.09577},
        {-0.93030, -0.00000, -0.36679},
        {-0.93030, -0.00000, -0.36679},
        {-0.72962, 0.00000, -0.68385},
        {-0.72962, 0.00000, -0.68385},
        {-0.42258, 0.00000, -0.90632},
        {-0.42258, 0.00000, -0.90632},
        {-0.29881, 0.29881, 0.90632},
        {-0.51592, 0.51592, 0.68385},
        {-0.65782, 0.65782, 0.36679},
        {-0.70386, 0.70386, -0.09577},
        {-0.65782, 0.65782, -0.36679},
        {-0.51592, 0.51592, -0.68385},
        {-0.29881, 0.29881, -0.90632},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, -1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {0.00000, 0.00000, 1.00000},
        {-0.00000, 0.42258, 0.90632},
        {-0.00000, 0.72962, 0.68385},
        {-0.00000, 0.93030, 0.36679},
        {-0.00000, 0.99540, -0.09577},
        {-0.00000, 0.93030, -0.36679},
        {-0.00000, 0.72962, -0.68385},
        {-0.00000, 0.42258, -0.90632},
        {0.99540, -0.00000, 0.09577},
        {0.70386, 0.70386, 0.09577},
        {0.70386, -0.70386, 0.09577},
        {0.00000, -0.99540, 0.09577},
        {-0.70386, -0.70386, 0.09577},
        {-0.99540, 0.00000, 0.09577},
        {-0.99540, 0.00000, 0.09577},
        {-0.70386, 0.70386, 0.09577},
        {-0.00000, 0.99540, 0.09577}
    };
    primitive->attribute.TANGENT = {
        {-0.70711, 0.70711, -0.00000, 1.00000},
        {-0.70711, 0.70711, -0.00000, 1.00000},
        {-0.70711, 0.70711, -0.00000, 1.00000},
        {-0.70711, 0.70711, 0.00000, 1.00000},
        {-0.70711, 0.70711, -0.00000, 1.00000},
        {-0.70711, 0.70711, 0.00000, 1.00000},
        {-0.70711, 0.70711, -0.00000, 1.00000},
        {0.00000, 1.00000, 0.00000, 1.00000},
        {0.00000, 1.00000, 0.00000, 1.00000},
        {0.00000, 1.00000, 0.00000, 1.00000},
        {0.00000, 1.00000, 0.00000, 1.00000},
        {-0.00000, 1.00000, -0.00000, 1.00000},
        {-0.00000, 1.00000, -0.00000, 1.00000},
        {0.00000, 1.00000, 0.00000, 1.00000},
        {0.70711, 0.70711, -0.00000, 1.00000},
        {0.70711, 0.70711, -0.00000, 1.00000},
        {0.70711, 0.70711, -0.00000, 1.00000},
        {0.70711, 0.70711, 0.00000, 1.00000},
        {0.70711, 0.70711, -0.00000, 1.00000},
        {0.70711, 0.70711, -0.00000, 1.00000},
        {0.70711, 0.70711, 0.00000, 1.00000},
        {1.00000, 0.00000, 0.00000, 1.00000},
        {1.00000, 0.00000, -0.00000, 1.00000},
        {1.00000, -0.00000, -0.00000, 1.00000},
        {1.00000, -0.00000, 0.00000, 1.00000},
        {1.00000, 0.00000, -0.00000, 1.00000},
        {1.00000, 0.00000, -0.00000, 1.00000},
        {1.00000, 0.00000, 0.00000, 1.00000},
        {0.70711, -0.70711, 0.00000, 1.00000},
        {0.70711, -0.70711, 0.00000, 1.00000},
        {0.70711, -0.70711, -0.00000, 1.00000},
        {0.70711, -0.70711, 0.00000, 1.00000},
        {0.70711, -0.70711, -0.00000, 1.00000},
        {0.70711, -0.70711, 0.00000, 1.00000},
        {0.70711, -0.70711, -0.00000, 1.00000},
        {0.31854, -0.93620, 0.14852, 1.00000},
        {-0.31854, -0.93620, -0.14852, 1.00000},
        {0.18638, -0.96214, 0.19885, 1.00000},
        {-0.18638, -0.96214, -0.19885, 1.00000},
        {0.05509, -0.98865, 0.13974, 1.00000},
        {-0.05509, -0.98865, -0.13974, 1.00000},
        {0.00380, -0.99921, -0.03945, 1.00000},
        {-0.00380, -0.99921, 0.03945, 1.00000},
        {0.05509, -0.98865, -0.13974, 1.00000},
        {-0.05509, -0.98865, 0.13974, 1.00000},
        {0.18638, -0.96214, -0.19885, 1.00000},
        {-0.18638, -0.96214, 0.19885, 1.00000},
        {0.31854, -0.93620, -0.14852, 1.00000},
        {-0.31854, -0.93620, 0.14852, 1.00000},
        {-0.70711, -0.70711, -0.00000, 1.00000},
        {-0.70711, -0.70711, -0.00000, 1.00000},
        {-0.70711, -0.70711, -0.00000, 1.00000},
        {-0.70711, -0.70711, 0.00000, 1.00000},
        {-0.70711, -0.70711, -0.00000, 1.00000},
        {-0.70711, -0.70711, 0.00000, 1.00000},
        {-0.70711, -0.70711, 0.00000, 1.00000},
        {0.38268, -0.92388, -0.00000, 1.00000},
        {0.92388, -0.38268, -0.00000, 1.00000},
        {0.92388, 0.38268, 0.00000, 1.00000},
        {0.38268, 0.92388, 0.00000, 1.00000},
        {-0.38268, 0.92388, 0.00000, 1.00000},
        {-0.92388, 0.38268, 0.00000, 1.00000},
        {-0.92388, -0.38268, -0.00000, 1.00000},
        {-0.38268, -0.92388, -0.00000, 1.00000},
        {0.38268, -0.92388, 0.00000, 1.00000},
        {0.92388, -0.38268, 0.00000, 1.00000},
        {0.92388, 0.38268, -0.00000, 1.00000},
        {0.38268, 0.92388, -0.00000, 1.00000},
        {-0.38268, 0.92388, -0.00000, 1.00000},
        {-0.92388, 0.38268, -0.00000, 1.00000},
        {-0.92388, -0.38268, 0.00000, 1.00000},
        {-0.38268, -0.92388, 0.00000, 1.00000},
        {-1.00000, 0.00000, -0.00000, 1.00000},
        {-1.00000, 0.00000, -0.00000, 1.00000},
        {-1.00000, 0.00000, -0.00000, 1.00000},
        {-1.00000, -0.00000, 0.00000, 1.00000},
        {-1.00000, -0.00000, -0.00000, 1.00000},
        {-1.00000, -0.00000, 0.00000, 1.00000},
        {-1.00000, 0.00000, 0.00000, 1.00000},
        {0.00000, 1.00000, 0.00000, 1.00000},
        {-0.70711, 0.70711, 0.00000, 1.00000},
        {0.70711, 0.70711, 0.00000, 1.00000},
        {1.00000, 0.00000, 0.00000, 1.00000},
        {0.70711, -0.70711, 0.00000, 1.00000},
        {0.00380, -0.99921, 0.03945, 1.00000},
        {-0.00380, -0.99921, -0.03945, 1.00000},
        {-0.70711, -0.70711, 0.00000, 1.00000},
        {-1.00000, -0.00000, 0.00000, 1.00000}
    };
    primitive->aabb = AABB(vec3(-1.f, -1.f, -2.f), vec3(1.f, 1.f, 2.f));
    primitive->SetupGLPrimitive();
    return primitive;
}

#endif