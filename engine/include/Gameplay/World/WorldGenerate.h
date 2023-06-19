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
    cCapsule->mesh->primitives.push_back(MakeCapsulePrimitive());

    Actor* aCube = target->AddChild<Actor>();
    aCube->localTransform.translation = vec3(0.f, -0.5f, 1.5f);
    aCube->localTransform.scaling = vec3(0.2f);
    MeshComponent* cCube = aCube->AddComponent<MeshComponent>();
    cCube->mesh = make_shared<Mesh>();
    cCube->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    
}
void GenSandbox(Actor* target)
{
    float edgeDistance = 100.f;

	Actor* aPlane = target->AddChild<Actor>();
    MeshComponent* cPlane = aPlane->AddComponent<MeshComponent>();
    cPlane->mesh = make_shared<Mesh>();
    cPlane->mesh->primitives.push_back(MakeQuadMeshPrimitive());
    aPlane->localTransform.scaling = vec3(edgeDistance);

    Actor* aCube1 = target->AddChild<Actor>();
    MeshComponent* cCube1 = aCube1->AddComponent<MeshComponent>();
    cCube1->mesh = make_shared<Mesh>();
    cCube1->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    aCube1->localTransform.translation = vec3(0.f, edgeDistance, edgeDistance*.1f);
    aCube1->localTransform.scaling = vec3(edgeDistance, 1.f, edgeDistance*.1f);
    CollisionComponent* cCollis1 = aCube1->AddComponent<CollisionComponent>();
    cCollis1->shape = make_shared<OBB>();

    Actor* aCube2 = target->AddChild<Actor>();
    MeshComponent* cCube2 = aCube2->AddComponent<MeshComponent>();
    cCube2->mesh = make_shared<Mesh>();
    cCube2->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    aCube2->localTransform.translation = vec3(edgeDistance, 0.f, edgeDistance*.1f);
    aCube2->localTransform.scaling = vec3(1.f, edgeDistance, edgeDistance*.1f);
    CollisionComponent* cCollis2 = aCube2->AddComponent<CollisionComponent>();
    cCollis2->shape = make_shared<OBB>();

    Actor* aCube3 = target->AddChild<Actor>();
    MeshComponent* cCube3 = aCube3->AddComponent<MeshComponent>();
    cCube3->mesh = make_shared<Mesh>();
    cCube3->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    aCube3->localTransform.translation = vec3(-edgeDistance, 0.f, edgeDistance*.1f);
    aCube3->localTransform.scaling = vec3(1.f, edgeDistance, edgeDistance*.1f);
    CollisionComponent* cCollis3 = aCube3->AddComponent<CollisionComponent>();
    cCollis3->shape = make_shared<OBB>();

    Actor* aCube4 = target->AddChild<Actor>();
    MeshComponent* cCube4 = aCube4->AddComponent<MeshComponent>();
    cCube4->mesh = make_shared<Mesh>();
    cCube4->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    aCube4->localTransform.translation = vec3(0.f, -edgeDistance, edgeDistance*.1f);
    aCube4->localTransform.scaling = vec3(edgeDistance, 1.f, edgeDistance*.1f);
    CollisionComponent* cCollis4 = aCube4->AddComponent<CollisionComponent>();
    cCollis4->shape = make_shared<OBB>();

    Actor* aCube5 = target->AddChild<Actor>();
    MeshComponent* cCube5 = aCube5->AddComponent<MeshComponent>();
    cCube5->mesh = make_shared<Mesh>();
    cCube5->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    aCube5->localTransform.translation = vec3(4.f, 0.f, 0.5f);
    aCube5->localTransform.rotation = EulerToQuat(vec3(0.f, 0.f, 30.f));
    aCube5->localTransform.scaling = vec3(1.f, .5f, .5f);
    CollisionComponent* cCollis5 = aCube5->AddComponent<CollisionComponent>();
    cCollis5->shape = make_shared<OBB>();

    Actor* aCube6 = target->AddChild<Actor>();
    MeshComponent* cCube6 = aCube6->AddComponent<MeshComponent>();
    cCube6->mesh = make_shared<Mesh>();
    cCube6->mesh->primitives.push_back(MakeCubeMeshPrimitive());
    aCube6->localTransform.translation = vec3(4.f, 0.f, 1.5f);
    aCube6->localTransform.rotation = EulerToQuat(vec3(0.f, 0.f, -30.f));
    aCube6->localTransform.scaling = vec3(1.f, .5f, .5f);
    CollisionComponent* cCollis6 = aCube6->AddComponent<CollisionComponent>();
    cCollis6->shape = make_shared<OBB>();;
    
    Actor* aCapsule1 = target->AddChild<Actor>();
    MeshComponent* cCapsule1 = aCapsule1->AddComponent<MeshComponent>();
    cCapsule1->mesh = make_shared<Mesh>();
    cCapsule1->mesh->primitives.push_back(MakeCapsulePrimitive());
    aCapsule1->localTransform.translation = vec3(0.f, 4.f, 1.f);
    aCapsule1->localTransform.rotation = EulerToQuat(vec3(90.f, 0.f, 30.f));
    CollisionComponent* cCollis7 = aCapsule1->AddComponent<CollisionComponent>();
    shared_ptr<Capsule> capsule = make_shared<Capsule>();;
    capsule->a = vec3(0.f, 0.f, 1.f);
    capsule->b = vec3(0.f, 0.f, -1.f);
    capsule->r = 1.f;
    cCollis7->shape = capsule;
}
#endif