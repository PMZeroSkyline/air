#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Gameplay/Component/SkinComponent.h"
#include "Gameplay/Component/MeshComponent.h"

class Renderable
{
public:
	shared_ptr<MeshPrimitive> primitive;
	MeshComponent* meshComponent;
	SkinComponent* skinComponent;
    bool operator<(const Renderable& rhs) const
    {
        return *primitive < *(rhs.primitive);
    }
};

#endif