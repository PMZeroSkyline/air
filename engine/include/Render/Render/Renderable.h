#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Gameplay/Component/SkinComponent.h"
#include "Gameplay/Component/MeshComponent.h"

class RenderablePrimitive
{
public:
	shared_ptr<MeshPrimitive> primitive;
};
class RenderableMesh : public RenderablePrimitive
{
public:
	MeshComponent* meshComponent;
};
class RenderableSkin : public RenderableMesh
{
public:
	SkinComponent* skinComponent;
};

#endif