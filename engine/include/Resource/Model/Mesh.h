#ifndef MESH_H
#define MESH_H

#include "MeshPrimitive.h"
#include "Resource/Container/Blob.h"
class Mesh
{
public:
    string name;
    vector<shared_ptr<MeshPrimitive>> primitives;
};
#endif