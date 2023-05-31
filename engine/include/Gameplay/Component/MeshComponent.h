#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "Component.h"
#include "Resource/Model/Mesh.h"
#include "Animation/SkinInstance.h"

class MeshComponent : public Component
{
public:
    shared_ptr<Mesh> mesh;
    SkinInstance* skinInstance = nullptr;
};



#endif