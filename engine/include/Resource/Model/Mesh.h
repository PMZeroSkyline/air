#ifndef MESH_H
#define MESH_H

#include "MeshPrimitive.h"
#include "Core/Container/WeakMap.h"
class Mesh
{
public:
    string name;
    vector<shared_ptr<MeshPrimitive>> primitives;
};

#endif