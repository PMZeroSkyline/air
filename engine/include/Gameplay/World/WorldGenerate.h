#ifndef WORLD_GENERATE
#define WORLD_GENERATE

#include "Gameplay/Object/Actor.h"
#include "Gameplay/Component/MeshComponent.h"
#include "Gameplay/Component/CollisionComponent.h"

void GenCapsuleMan(Actor* target)
{
    Actor* aCapsule = target->AddChild<Actor>();
    aCapsule->localTransform.translation = vec3(0.f, 0.f, 1.f);
    aCapsule->localTransform.scaling = vec3(0.5f);
    MeshComponent* cCapsule = aCapsule->AddComponent<MeshComponent>();
    cCapsule->mesh = make_shared<Mesh>();
	shared_ptr<Material> material = make_shared<Material>();
    material->shader = MakeShaderFromRes("uv_grid");
    cCapsule->mesh->primitives.push_back(MakeCapsulePrimitive(material));

    Actor* aCube = target->AddChild<Actor>();
    aCube->localTransform.translation = vec3(0.f, -0.5f, 1.5f);
    aCube->localTransform.scaling = vec3(0.2f);
    MeshComponent* cCube = aCube->AddComponent<MeshComponent>();
    cCube->mesh = make_shared<Mesh>();
    cCube->mesh->primitives.push_back(MakeCubeMeshPrimitive(material));
    
}
void GenSandbox(Actor* target)
{
    shared_ptr<Material> world_grid = make_shared<Material>();
    world_grid->shader = MakeShaderFromRes("world_grid");
    shared_ptr<Material> world_normal = make_shared<Material>();
    world_normal->shader = MakeShaderFromRes("world_normal");

    float edgeDistance = 10.f;

	Actor* aPlane = target->AddChild<Actor>();
    MeshComponent* cPlane = aPlane->AddComponent<MeshComponent>();
    cPlane->mesh = make_shared<Mesh>();
    cPlane->mesh->primitives.push_back(MakeQuadMeshPrimitive(world_grid));

    aPlane->localTransform.scaling = vec3(edgeDistance);

    Actor* aCube1 = target->AddChild<Actor>();
    MeshComponent* cCube1 = aCube1->AddComponent<MeshComponent>();
    cCube1->mesh = make_shared<Mesh>();
    cCube1->mesh->primitives.push_back(MakeCubeMeshPrimitive(world_grid));
    aCube1->localTransform.translation = vec3(0.f, edgeDistance, edgeDistance*.1f);
    aCube1->localTransform.scaling = vec3(edgeDistance, 1.f, edgeDistance*.1f);
    CollisionComponent* cCollis1 = aCube1->AddComponent<CollisionComponent>();
    shared_ptr<OBB> sOBB1 = make_shared<OBB>();
    cCollis1->a = sOBB1;

    Actor* aCube2 = target->AddChild<Actor>();
    MeshComponent* cCube2 = aCube2->AddComponent<MeshComponent>();
    cCube2->mesh = make_shared<Mesh>();
    cCube2->mesh->primitives.push_back(MakeCubeMeshPrimitive(world_grid));
    aCube2->localTransform.translation = vec3(edgeDistance, 0.f, edgeDistance*.1f);
    aCube2->localTransform.scaling = vec3(1.f, edgeDistance, edgeDistance*.1f);
    CollisionComponent* cCollis2 = aCube2->AddComponent<CollisionComponent>();
    shared_ptr<OBB> sOBB2 = make_shared<OBB>();
    cCollis2->a = sOBB2;

    Actor* aCube3 = target->AddChild<Actor>();
    MeshComponent* cCube3 = aCube3->AddComponent<MeshComponent>();
    cCube3->mesh = make_shared<Mesh>();
    cCube3->mesh->primitives.push_back(MakeCubeMeshPrimitive(world_grid));
    aCube3->localTransform.translation = vec3(-edgeDistance, 0.f, edgeDistance*.1f);
    aCube3->localTransform.scaling = vec3(1.f, edgeDistance, edgeDistance*.1f);
    CollisionComponent* cCollis3 = aCube3->AddComponent<CollisionComponent>();
    shared_ptr<OBB> sOBB3 = make_shared<OBB>();
    cCollis3->a = sOBB3;

    Actor* aCube4 = target->AddChild<Actor>();
    MeshComponent* cCube4 = aCube4->AddComponent<MeshComponent>();
    cCube4->mesh = make_shared<Mesh>();
    cCube4->mesh->primitives.push_back(MakeCubeMeshPrimitive(world_grid));
    aCube4->localTransform.translation = vec3(0.f, -edgeDistance, edgeDistance*.1f);
    aCube4->localTransform.scaling = vec3(edgeDistance, 1.f, edgeDistance*.1f);
    CollisionComponent* cCollis4 = aCube4->AddComponent<CollisionComponent>();
    shared_ptr<OBB> sOBB4 = make_shared<OBB>();
    cCollis4->a = sOBB4;

    Actor* aCube5 = target->AddChild<Actor>();
    MeshComponent* cCube5 = aCube5->AddComponent<MeshComponent>();
    cCube5->mesh = make_shared<Mesh>();
    cCube5->mesh->primitives.push_back(MakeCubeMeshPrimitive(world_normal));
    aCube5->localTransform.translation = vec3(4.f, 0.f, 1.f);
    aCube5->localTransform.scaling = vec3(.5f);
    CollisionComponent* cCollis5 = aCube5->AddComponent<CollisionComponent>();
    shared_ptr<OBB> sOBB5 = make_shared<OBB>();
    cCollis5->a = sOBB5;
    
}

#endif