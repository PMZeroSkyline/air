#ifndef ACTOR_H
#define ACTOR_H

#include "Entity.h"
#include "Gameplay/Component/AnimationNodeComponent.h"
#include "Core/Parse/StringParse.h"

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


#endif