#ifndef ACTOR_H
#define ACTOR_H

#include "Entity.h"
#include "Gameplay/Component/AnimationNodeComponent.h"
#include "Core/Parse/StringParse.h"
#include "Gameplay/Component/MeshComponent.h"

class Actor : public Entity
{
public:
	bool isDirty = true;
	mat4 worldMatrix;
	Transform localTransform;
	
	vec3 GetForwardVector() const
	{
		const vec4 c = worldMatrix.column(0);
		return vec3(c.x, c.y, c.z);
	}
	vec3 GetUpVector() const
	{
		const vec4 c = worldMatrix.column(2);
		return vec3(c.x, c.y, c.z);
	}
	vec3 GetRightVector() const
	{
		const vec4 c = worldMatrix.column(1);
		return vec3(c.x, c.y, c.z);
	}
	void SetWorldMatrix(const mat4& m)
	{
		mat4 IW = parent ? ((Actor*)parent)->worldMatrix.inverse() : mat4();
		mat4 localMatrix = IW * m;
		localTransform = Transform(localMatrix);
		ResetWorldMatrix(true);
	}
	void ResetWorldMatrix(bool isForce = false, Actor* rootMotionTarget = nullptr)
	{
		AnimationNodeComponent* animationNodeComponent = GetComponent<AnimationNodeComponent>();
		bool hasAnimation = false;
		Transform animationTransform;
		if (animationNodeComponent)
		{
			animationTransform = animationNodeComponent->GetAnimationTransform();
			hasAnimation = animationTransform != Transform();
		}
		bool isReset = isForce || isDirty || hasAnimation;
		if (isReset)
		{
			mat4 world = parent ? ((Actor*)parent)->worldMatrix : mat4();
			if (rootMotionTarget && hasAnimation && name == "root")
			{
				mat4 preAnimationMatrix = world * animationNodeComponent->GetAnimationTransform(-window->deltaTime).ToMatrix();
				mat4 rootMotionMatrix = (world * animationTransform.ToMatrix()) / preAnimationMatrix;
				rootMotionTarget->SetWorldMatrix(rootMotionMatrix * rootMotionTarget->worldMatrix);
				worldMatrix = world * localTransform.ToMatrix();
			}
			else
			{
				worldMatrix = world * (hasAnimation ? animationTransform.ToMatrix() : localTransform.ToMatrix());
			}
			isDirty = false;
		}
		for (int i = 0; i < children.size(); i++)
		{
			((Actor*)children[i])->ResetWorldMatrix(isReset, rootMotionTarget);
		}
	}
};

void GenCapsuleMan(Actor* target)
{
    Actor* aCapsule = target->AddChild<Actor>();
    aCapsule->localTransform.translation = vec3(0.f, 0.f, 1.f);
    aCapsule->localTransform.scaling = vec3(0.5f);
    MeshComponent* cCapsule = aCapsule->AddComponent<MeshComponent>();
    cCapsule->mesh = make_shared<Mesh>();
	shared_ptr<Material> material = make_shared<Material>();
    material->shader = GetPresetShader("sandbox");
    cCapsule->mesh->primitives.push_back(MakeCapsulePrimitive(material));

    Actor* aCube = target->AddChild<Actor>();
    aCube->localTransform.translation = vec3(0.f, -0.5f, 1.5f);
    aCube->localTransform.scaling = vec3(0.2f);
    MeshComponent* cCube = aCube->AddComponent<MeshComponent>();
    cCube->mesh = make_shared<Mesh>();
    cCube->mesh->primitives.push_back(MakeCubeMeshPrimitive(material));
}
#endif